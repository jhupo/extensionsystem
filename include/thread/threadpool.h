#ifndef __EXTENSION_THREADPOOL_H__
#define __EXTENSION_THREADPOOL_H__

#include <extension_global.h>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <basicvariant/basicvariant.h>

namespace extension{

    namespace core{

        class ThreadPoolPrivate;

        class EXTENSION_EXPORT ThreadPool 
        {
            DISABLE_COPY(ThreadPool)
            DECLARE_PRIVATE(ThreadPool)
        public:

            typedef boost::function<void(const BasicVariant&)> TaskCallback;

            static ThreadPool* inst();

            void addTask(const std::string& iid, const TaskCallback& call, const BasicVariant& var = basic_variant_null);
            bool tryStartTask(const TaskCallback& call, const BasicVariant& var = basic_variant_null);
            
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
            const boost::shared_ptr<ThreadPoolPrivate>              d_ptr;
        };

    }

}


#endif