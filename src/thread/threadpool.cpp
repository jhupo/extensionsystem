#include <thread/threadpool.h>



namespace extension{

    namespace core{

        class ThreadPoolPrivate
        {
            DECLARE_PUBLIC(ThreadPool)
        public:

            ThreadPoolPrivate(ThreadPool* q);
            ~ThreadPoolPrivate();

            ThreadPool*const                q_ptr;
        };


    }


}