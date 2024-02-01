#ifndef __EXTENSION_THREADPOOL_H__
#define __EXTENSION_THREADPOOL_H__

#include <extension_global.h>
#include <utils/extensionutils.h>

#include <functional>

namespace extension{

    class ThreadPoolPrivate;

    class EXTENSION_EXPORT ThreadPool 
    {
        DISABLE_COPY(ThreadPool)
        DECLARE_PRIVATE(ThreadPool)
        DECLARE_SHARED_PTR(ThreadPool)
    public:

        typedef std::function<void(void*)> TaskCallback;

        void addTask(const std::string& iid, const TaskCallback& call, void* args = nullptr);
        bool tryStartTask(const TaskCallback& call, void* args = nullptr);
        
        void clear();
        void cancel(const std::string& iid);

        int expiryTimeout() const;
        void setExpiryTimeout(int expiryTimeout);

        int maxThreadCount() const;
        void setMaxThreadCount(int maxThreadCount);

        int activeThreadCount() const;

        bool waitForDone(int msecs = -1);

        ThreadPool();
        virtual~ThreadPool();

    private:
        const std::shared_ptr<ThreadPoolPrivate>              d_ptr;
    };

    DECLARE_SINGLETON(ThreadPool);
}


#endif