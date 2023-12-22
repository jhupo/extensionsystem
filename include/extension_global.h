#ifndef __EXTENSION_GLOBAL_H__
#define __EXTENSION_GLOBAL_H__

#include <extensiondefs.h>

#if defined(EXTENSION_LIBRARY)
#  define EXTENSION_EXPORT DECL_EXPORT
#elif defined(EXTENSION_STATIC_LIBRARY)
#  define EXTENSION_EXPORT
#else
#  define EXTENSION_EXPORT DECL_IMPORT
#endif

#endif 