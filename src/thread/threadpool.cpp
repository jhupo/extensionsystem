#include <thread/threadpool.h>
#include <thread/thread.h>
#include <logger/logger.h>

#include <string>
#include <mutex>
#include <set>
#include <thread>
#include <vector>
#include <condition_variable>

namespace extension{

    struct ThreadTask
    {
        void*                           args;
        std::string                     iid;
        ThreadPool::TaskCallback        cb;
    };

    class ThreadPoolThread : public Thread
    {
    public:
        ThreadPoolPrivate*const     _manager;
        ThreadTask*                 _task;
        std::condition_variable     _variable;  
    public:
        ThreadPoolThread(ThreadPoolPrivate* manager);
        virtual~ThreadPoolThread();
    protected:
        void run()override;
        void registerThreadInactive();
    };

    class ThreadPoolPrivate
    {
        DECLARE_PUBLIC(ThreadPool)
    public:

        ThreadPoolPrivate(ThreadPool* q);
        ~ThreadPoolPrivate();

        bool tooManyThreadsActive()const;
        int activeThreadCount()const;
        bool waitForDone(int msec);
        void reset();
        void clear();
        void startThread(ThreadTask* task = NULL);
        bool tryStartTask(ThreadTask* task);
        void tryToStartMoreThreads();
        bool stealRunnable(const char* iid);

        ThreadPool*const                                        q_ptr;
        mutable std::mutex                                      _mutex;
        std::set<ThreadPoolThread*>                             _allThreads;
        std::vector<ThreadPoolThread*>                          _waitingThreads;
        std::vector<ThreadPoolThread*>                          _expiredThreads;
        std::vector<ThreadTask*>                                _tasks;
        std::condition_variable                                 _variable;
        bool                                                    _isExiting;
        int                                                     _maxThreadCount;
        int                                                     _activeThreads;
        int                                                     _expiryTimeout;
        int                                                     _reservedThreads;
    };

    ThreadPoolThread::ThreadPoolThread(ThreadPoolPrivate* manager)
        : _manager(manager)
        , _task(nullptr)
    {

    }

    ThreadPoolThread::~ThreadPoolThread()
    {

    }

    void ThreadPoolThread::run()
    {
        std::unique_lock<std::mutex> locker(_manager->_mutex);
        for(;;){
            ThreadTask* r = _task;
            _task = NULL;
            do{
                if(r){
                    locker.unlock();
                    try{
                        r->cb(r->args);
                    }
                    catch(...){
                        LOG_ERROR(threadpool) << "This is not supported, exceptions thrown in worker threads must be caught before control returns.";
                        registerThreadInactive();
                        throw;
                    }
                    delete r;
                    r = NULL;
                    locker.lock();
                }
                if(_manager->tooManyThreadsActive()){
                    break;
                }
                r = !_manager->_tasks.empty() ? _manager->_tasks.front() : NULL; 
                if(r){
                    _manager->_tasks.erase(_manager->_tasks.begin());
                }
            }while(NULL != r);
            if(_manager->_isExiting){
                registerThreadInactive();
                break;
            }
            bool expired = _manager->tooManyThreadsActive();
            if(!expired){
                _manager->_waitingThreads.push_back(this);
                registerThreadInactive();
                _variable.wait_for(locker,std::chrono::milliseconds(_manager->_expiryTimeout));
                ++_manager->_activeThreads;
                std::vector<ThreadPoolThread*>::iterator iter =
                    std::find(_manager->_waitingThreads.begin(), _manager->_waitingThreads.end(), this);
                if(iter != _manager->_waitingThreads.end()){
                    _manager->_waitingThreads.erase(iter);
                    expired = true;
                }
            }
            if(expired){
                _manager->_expiredThreads.push_back(this);
                registerThreadInactive();
                break;
            }
        }
    }

    void ThreadPoolThread::registerThreadInactive()
    {
        if (--_manager->_activeThreads == 0){
            _manager->_variable.notify_all();
        }
    }

    ThreadPoolPrivate::ThreadPoolPrivate(ThreadPool* q)
        : q_ptr(q)
        , _isExiting(false)
        , _maxThreadCount(std::thread::hardware_concurrency())
        , _activeThreads(0)
        , _expiryTimeout(30000)
        , _reservedThreads(0)
    {

    }

    ThreadPoolPrivate::~ThreadPoolPrivate()
    {

    }

    bool ThreadPoolPrivate::tooManyThreadsActive()const
    {
        const int activeThreadCount = this->activeThreadCount();
        return activeThreadCount > _maxThreadCount && (activeThreadCount - _reservedThreads) > 1;
    }

    int ThreadPoolPrivate::activeThreadCount()const
    {
        return (_allThreads.size()
                - _expiredThreads.size()
                - _waitingThreads.size()
                + _reservedThreads);
    }

    bool ThreadPoolPrivate::waitForDone(int msec)
    {
        std::unique_lock<std::mutex> locker(_mutex);
        if(0 > msec){
            while (!(_tasks.empty() && _activeThreads == 0)){
                _variable.wait(locker);
            }
        }else{
            // boost::posix_time::ptime timer = boost::posix_time::microsec_clock::universal_time();
            std::chrono::_V2::steady_clock::time_point timer = std::chrono::steady_clock::now();
            int t;
            while (!(_tasks.empty() && _activeThreads == 0) &&
               ((t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timer).count()) > 0))
            {
                _variable.wait_for(locker, std::chrono::milliseconds(t));
            }
        }
        return _tasks.empty() && _activeThreads == 0;
    }

    void ThreadPoolPrivate::reset()
    {
        std::unique_lock<std::mutex> locker(_mutex);
        _isExiting = true;
        while(!_allThreads.empty())
        {
            std::set<ThreadPoolThread*> allThreadsCopy;
            allThreadsCopy.swap(_allThreads);
            locker.unlock();
            std::set<ThreadPoolThread*>::const_iterator iter = allThreadsCopy.begin();
            for(;iter != allThreadsCopy.end(); ++iter) {
                ThreadPoolThread* thread = *iter;
                thread->_variable.notify_all();
            }
            locker.lock();
        }
        _waitingThreads.clear();
        std::vector<ThreadPoolThread*>::iterator iter(_expiredThreads.begin());
        while(iter != _expiredThreads.end()){
            delete *iter;
            iter = _expiredThreads.erase(iter);
        }
        _isExiting = false;
    }

    void ThreadPoolPrivate::clear()
    {
        std::vector<ThreadTask*>::iterator iter(_tasks.begin());
        for(;iter != _tasks.end(); ++iter){
            delete *iter;
            iter = _tasks.erase(iter);
        }
    }

    void ThreadPoolPrivate::startThread(ThreadTask* task)
    {
        ThreadPoolThread* thread = new ThreadPoolThread(this);
        thread->_task = task;
        thread->start();
        _allThreads.insert(thread);
        ++_activeThreads;
    }

    bool ThreadPoolPrivate::tryStartTask(ThreadTask* task)
    {
        if(_allThreads.empty()){
            startThread(task);
            return true;
        }
        if (activeThreadCount() >= _maxThreadCount){
            return false;
        }
        if (_waitingThreads.size() > 0) {
            _tasks.push_back(task);
            ThreadPoolThread* thread = _waitingThreads.front();
            thread->_variable.notify_one();
            _waitingThreads.erase(_waitingThreads.begin());
            return true;
        }
        if(!_expiredThreads.empty()){
            ThreadPoolThread* thread = _expiredThreads.front();
            thread->_task = task;
            _expiredThreads.erase(_expiredThreads.begin());
            thread->start();
            return true;
        }
        startThread(task);
        return true;
    }

    void ThreadPoolPrivate::tryToStartMoreThreads()
    {
        while (!_tasks.empty() && tryStartTask(_tasks.front())){
            _tasks.erase(_tasks.begin());
        }
    }

    struct _thread_pool_task_find_if_cb_
    {
        std::string _iid;
        _thread_pool_task_find_if_cb_(const std::string& iid):_iid(iid){}
        bool operator()(ThreadTask* task) const
        {
            return _iid == task->iid;
        }
    };

    bool ThreadPoolPrivate::stealRunnable(const char* iid)
    {
        std::vector<ThreadTask*>::iterator iter = std::find_if(_tasks.begin(),_tasks.end(),_thread_pool_task_find_if_cb_(iid));
        if(iter != _tasks.end()){
            delete *iter;
            _tasks.erase(iter);
            return true;
        }
        return false;
    }

    ThreadPool::ThreadPool()
        : d_ptr(new ThreadPoolPrivate(this))
    {

    }

    ThreadPool::~ThreadPool()
    {
        waitForDone();
    }

    void ThreadPool::addTask(const std::string& iid, const TaskCallback& call, void* args)
    {
        D_P(ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        if(d->_tasks.end() != std::find_if(d->_tasks.begin(),d->_tasks.end(),_thread_pool_task_find_if_cb_(iid))){
            return;
        }
        ThreadTask* task = new ThreadTask;
        task->args = args;
        task->cb = call;
        task->iid = iid;
        if(!d->tryStartTask(task)){
            d->_tasks.push_back(task);
            if(!d->_waitingThreads.empty()){
                ThreadPoolThread* thread = d->_waitingThreads.front();
                thread->_variable.notify_one();
                d->_waitingThreads.erase(d->_waitingThreads.begin());
            }
        }
    }

    bool ThreadPool::tryStartTask(const TaskCallback& call, void* args)
    {
        D_P(ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        if(!d->_allThreads.empty() && d->activeThreadCount() >= d->_maxThreadCount){
            return false;
        }
        ThreadTask* task = new ThreadTask;
        task->args = args;
        task->cb = call;
        return d->tryStartTask(task);
    }

    void ThreadPool::clear()
    {
        D_P(ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        d->clear();
    }

    void ThreadPool::cancel(const std::string& iid)
    {
        D_P(ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        d->stealRunnable(iid.c_str());
    }

    int ThreadPool::expiryTimeout() const
    {
        D_P(const ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        return d->_expiryTimeout;
    }

    void ThreadPool::setExpiryTimeout(int expiryTimeout)
    {
        D_P(ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        d->_expiryTimeout = expiryTimeout;
    }

    int ThreadPool::maxThreadCount() const
    {
        D_P(const ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        return d->_maxThreadCount;
    }

    void ThreadPool::setMaxThreadCount(int maxThreadCount)
    {
        D_P(ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        if(d->_maxThreadCount != maxThreadCount){
            d->_maxThreadCount = maxThreadCount;
            d->tryToStartMoreThreads();
        }
    }

    int ThreadPool::activeThreadCount() const
    {
        D_P(const ThreadPool);
        std::unique_lock<std::mutex> locker(d->_mutex);
        return d->activeThreadCount();
    }

    bool ThreadPool::waitForDone(int msecs)
    {
        D_P(ThreadPool);
        bool rc = d->waitForDone(msecs);
        if (rc){
            d->reset();
        }
        return rc;
    }


}