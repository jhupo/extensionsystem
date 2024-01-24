#ifndef __EXTENSION_THREAD_H__
#define __EXTENSION_THREAD_H__

#include <extension_global.h>

#include <string>

namespace extension{

    class ThreadPrivate;

    class EXTENSION_EXPORT Thread
    {
        DISABLE_COPY(Thread)
        DECLARE_PRIVATE(Thread)
        DECLARE_SHARED_PTR(Thread)
    public:

        Thread(const std::string& name = "");
        virtual~Thread();

        void requestInterruption();
        bool isInterruptionRequested() const;

        bool isRunning() const;

        void start();

    protected:
        virtual void run();
    public:
        static void sleep(unsigned long);
        static void msleep(unsigned long);
        static void usleep(unsigned long);
        static uint64_t this_thread_id();
        static std::string this_thread_name();
        static void set_this_thread_name(const std::string&);
    private:
        friend class ThreadPrivate;
        const std::shared_ptr<ThreadPrivate> d_ptr;
    };

}


#endif