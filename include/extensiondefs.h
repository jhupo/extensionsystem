#ifndef __EXTENSIONDEFS_H__
#define __EXTENSIONDEFS_H__

#if defined(WIN32)
#define DECL_EXPORT __declspec(dllexport)
#define DECL_IMPORT __declspec(dllimport)
#elif defined(__linux__) || defined(__unix__)
#define DECL_EXPORT __attribute__((visibility("default")))
#define DECL_IMPORT __attribute__((visibility("default")))
#else
#error Unsupported platform
#endif

#if __cplusplus > 201103L
#define DECL_MODERNITY_COMPILER
#endif

#ifdef DECL_MODERNITY_COMPILER
#define COMPILER_DELETE_MEMBERS
#endif

#ifdef DECL_MODERNITY_COMPILER
#define DECL_EQ_DELETE = delete
#define DECL_EQ_DEFAULT = default
#define DECL_EQ_OVERRIDE override
#define DECL_EQ_FINAL final
#define DECL_EQ_NOEXCEPT noexcept
#define DECL_EQ_CONSTEXPR constexpr
#define DECL_EQ_NULLPTR nullptr
#else
#define DECL_EQ_DELETE
#define DECL_EQ_DEFAULT
#define DECL_EQ_OVERRIDE
#define DECL_EQ_FINAL
#define DECL_EQ_NOEXCEPT
#define DECL_EQ_CONSTEXPR
#define DECL_EQ_NULLPTR NULL
#endif

#define UNUSED(x) (void)x

#define DISABLE_COPY(Class) \
    Class(const Class &) DECL_EQ_DELETE;\
    Class &operator=(const Class &) DECL_EQ_DELETE;

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
        typedef boost::shared_ptr<Class> ptr; \
        typedef boost::shared_ptr<const Class> const_ptr; \
    private:

#if (defined(__unix__) || defined(_WIN32)) && (defined(__GNUC__) || defined(__clang__))
#  define PLUGIN_METADATA_SECTION \
    __attribute__((section(".metadata"))) __attribute__((used))
#elif defined(__APPLE__)
#  define PLUGIN_METADATA_SECTION \
    __attribute__((section("__TEXT,metadata"))) __attribute__((used))
#elif defined(_MSC_VER)
#pragma section(".mymetadata",read,shared)
#  define PLUGIN_METADATA_SECTION \
    __declspec(allocate(".metadata"))
#else
#  define PLUGIN_METADATA_SECTION
#endif


#endif