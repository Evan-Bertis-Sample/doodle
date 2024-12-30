#ifndef __NATIVE_DEBUG_H__
#define __NATIVE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h> // for exit()
#include <stdio.h>  // for printf()

#define NATIVE_LOG_ENABLED
#define NATIVE_ALLOW_FATAL_ERRORS
#define NATIVE_MSG_PREFIX "[doodle-platform][native]"

#ifdef NATIVE_LOG_ENABLED
#define NATIVE_LOG(...) printf(NATIVE_MSG_PREFIX " " __VA_ARGS__)
#define NATIVE_LOG_ERROR(...) printf(NATIVE_MSG_PREFIX " [ERROR] " __VA_ARGS__)
#else
#define NATIVE_LOG(...) void(0)
#define NATIVE_LOG_ERROR(...) void(0)
#endif

#ifdef NATIVE_ALLOW_FATAL_ERRORS
#define NATIVE_FATAL_ERROR(...) { NATIVE_LOG_ERROR(__VA_ARGS__); exit(1); }
#else
#define NATIVE_FATAL_ERROR(...) NATIVE_LOG_ERROR(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __NATIVE_DEBUG_H__