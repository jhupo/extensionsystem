#include <thread/thread.h>
#include <logger/logger.h>
#include <utils/extensionutils.h>

#include <mutex>
#include <thread>

namespace extension{

    class ThreadPrivate
    {
        DECLARE_PUBLIC(Thread)
    public:
        ThreadPrivate(Thread* q);
        ~ThreadPrivate();

        void start();

        void wait();

        Thread*const                    q_ptr;
        bool                            _running;
        bool                            _interruptionRequested;
        mutable std::mutex              _mutex;
        std::string                     _name;
        std::shared_ptr<std::thread>    _thread;
    };

    ThreadPrivate::ThreadPrivate(Thread* q)
        : q_ptr(q)
        , _running(false)
        , _interruptionRequested(false)
    {
        
    }

    ThreadPrivate::~ThreadPrivate()
    {
        wait();
    }

    void ThreadPrivate::start()
    {
        {
            std::lock_guard<std::mutex> locker(_mutex);
            Thread::set_this_thread_name(_name);
        }

        q_ptr->run();

        {
            std::lock_guard<std::mutex> locker(_mutex);
            _running = false;
        }

    }

    void ThreadPrivate::wait()
    {
        if(_thread){
            if(_thread->joinable()){
                _thread->join();
            }
        }
    }

    Thread::Thread(const std::string& name)
        : d_ptr(new ThreadPrivate(this))
    {
        d_ptr->_name = name;
    }

    Thread::~Thread()
    {
        
    }

    void Thread::requestInterruption()
    {
        D_P(Thread);
        std::unique_lock<std::mutex> locker(d->_mutex);
        d->_interruptionRequested = true;
    }

    bool Thread::isInterruptionRequested() const
    {
        D_P(const Thread);
        std::unique_lock<std::mutex> locker(d->_mutex);
        return d->_interruptionRequested;
    }

    bool Thread::isRunning() const
    {
        D_P(const Thread);
        std::unique_lock<std::mutex> locker(d->_mutex);
        return d->_running;
    }

    void Thread::start()
    {
        D_P(Thread);
        std::unique_lock<std::mutex> locker(d->_mutex);
        if(d->_running){
            return;
        }
        d->_running = true;
        d->_interruptionRequested = false;
        d->_thread.reset(new std::thread(&ThreadPrivate::start, d));
    }

    void Thread::run()
    {

    }

    void Thread::sleep(unsigned long secs)
    {
        std::this_thread::sleep_for(std::chrono::seconds(secs));
    }

    void Thread::msleep(unsigned long msecs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msecs));
    }

    void Thread::usleep(unsigned long usecs)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(usecs));
    }

    uint64_t Thread::this_thread_id()
    {
        return std::hash<std::thread::id>()(std::this_thread::get_id());
    }

    std::string Thread::this_thread_name()
    {
        return this_thread::this_thread_name();
    }

    void Thread::set_this_thread_name(const std::string& name)
    {
        this_thread::set_this_thread_name(name);
    }




}