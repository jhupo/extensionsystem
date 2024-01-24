#ifndef __EXTENSIONDEFS_H__
#define __EXTENSIONDEFS_H__

#include <memory>

#if defined(WIN32)
#define DECL_EXPORT __declspec(dllexport)
#define DECL_IMPORT __declspec(dllimport)
#elif defined(__linux__) || defined(__unix__)
#define DECL_EXPORT __attribute__((visibility("default")))
#define DECL_IMPORT __attribute__((visibility("default")))
#else
#error Unsupported platform
#endif

#define UNUSED(x) (void)x

#define DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

template <typename T> static inline T *GetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper> static inline typename Wrapper::element_type* GetPtrHelper(const Wrapper &p) { return p.get(); }

#define DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(GetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(GetPtrHelper(d_ptr)); } 

#define DECLARE_PUBLIC(Class)                                    \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); }

#define D_P(Class) Class##Private * const d = d_func()
#define Q_P(Class) Class * const q = q_func()

#define DECLARE_SHARED_PTR(Class)   \
    public: \
        typedef std::shared_ptr<Class> ptr; \
        typedef std::weak_ptr<Class> weak_ptr; \
    private:

#if (defined(__unix__) || defined(_WIN32)) && (defined(__GNUC__) || defined(__clang__))
#  define PLUGIN_METADATA_SECTION \
    __attribute__((section(".metadata"))) __attribute__((used))
#elif defined(__APPLE__)
#  define PLUGIN_METADATA_SECTION \
    __attribute__((section("__TEXT,metadata"))) __attribute__((used))
#elif defined(_MSC_VER)
#pragma section(".metadata",read,shared)
#  define PLUGIN_METADATA_SECTION \
    __declspec(allocate(".metadata"))
#else
#  define PLUGIN_METADATA_SECTION
#endif



namespace extension{
template<typename T>
class DECL_EXPORT Singleton\
{\
DISABLE_COPY(Singleton)\
public:\
static T* inst(){static T v;return &v;}\
};
}

#define DECLARE_SINGLETON(Class,Inst)\
    typedef extension::Singleton<Class> Inst

#endif