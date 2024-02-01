#ifndef __EXTENSION_UTILS_H__
#define __EXTENSION_UTILS_H__

#include <extension_global.h>


namespace extension{

    template<typename T>
    class EXTENSION_EXPORT Singleton
    {
        DISABLE_COPY(Singleton)
    public:
        static T* inst(){static T v;return &v;}
    };
#define DECLARE_SINGLETON(Class)\
    typedef extension::Singleton<Class> SGL##Class


    class EXTENSION_EXPORT this_thread
    {
    public:
        static void set_this_thread_name(const std::string&);
        static std::string this_thread_name();
    };

    class EXTENSION_EXPORT Utils
    {
    public:
        static uint64_t current_steady_time_ms();
    };

}


#endif