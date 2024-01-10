#ifndef __EXTENSION_THREADPOOL_H__
#define __EXTENSION_THREADPOOL_H__

#include <extension_global.h>

#include <functional>

namespace extension{

    namespace core{

        class ThreadPoolPrivate;

        class EXTENSION_EXPORT ThreadPool 
        {
            DISABLE_COPY(ThreadPool)
            DECLARE_PRIVATE(ThreadPool)
        public:

            typedef std::function<void(void*)> TaskCallback;

            static ThreadPool* inst();

            void addTask(const std::string& iid, const TaskCallback& call, void* var = nullptr);
            bool tryStartTask(const TaskCallback& call, void* var = nullptr);
            
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

    }

}


#endif