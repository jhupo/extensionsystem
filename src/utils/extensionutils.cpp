#include <utils/extensionutils.h>

#if (defined(__unix__) || defined(__linux__))
#include <pthread.h>
#include <sys/prctl.h>
#define LINUX
#elif (defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#define WINDOWS
#endif

#include <string>
#include <chrono>

namespace extension{

    namespace{
#ifdef WINDOWS
#pragma pack(push,8)
            typedef struct THREAD_NAME_INFO
            {
                DWORD dwType; 
                LPCSTR szName; 
                DWORD dwThreadID; 
                DWORD dwFlags; 
            } THREAD_NAME_INFO;
#pragma pack(pop)
#endif
    }

    void this_thread::set_this_thread_name(const std::string& name)
    {
#ifdef LINUX
#if (__GLIBC__ == 2 && __GLIBC_MINOR__ < 12)
        prctl(PR_SET_NAME, name.c_str());
#endif
        pthread_setname_np(pthread_self(), name.c_str());
#else
        THREAD_NAME_INFO info;
        info.dwType = 0x1000;
        info.szName = name.c_str();
        info.dwThreadID = -1;
        info.dwFlags = 0;
        __try{
            const DWORD MS_VC_EXCEPTION = 0x406D1388;
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        }__except (EXCEPTION_EXECUTE_HANDLER) {
        }
#endif
    }

    std::string this_thread::this_thread_name()
    {
        std::string result;
#ifdef LINUX
        char threadName[16];
        pthread_getname_np(pthread_self(), threadName, sizeof(threadName));
        result = threadName;
#else
        THREAD_NAME_INFO info;
        info.dwType = 0x1000;
        info.szName = nullptr;
        info.dwThreadID = -1;
        info.dwFlags = 0;
        __try {
        const DWORD MS_VC_EXCEPTION = 0x406D1388;
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        result = info.szName;
        } __except (EXCEPTION_EXECUTE_HANDLER) {
        }
#endif
        return result;
    }

    uint64_t Utils::current_steady_time_ms()
    {
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now().time_since_epoch()).count());
    }


}