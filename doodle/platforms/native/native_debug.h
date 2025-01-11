#ifndef __NATIVE_DEBUG_H__
#define __NATIVE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <doodle/core/doodle_debug.h>  // includes the runtime-based DOODLE_FILENAME
#include <stdio.h>                     // for printf()
#include <stdlib.h>                    // for exit()

#define NATIVE_LOG_ENABLED
#define NATIVE_ALLOW_FATAL_ERRORS

// Just a simple prefix for native logs
#define NATIVE_MSG_PREFIX "[native]"

#define NATIVE_COLOR DOODLE_PLATFORM_COLOR
#define NATIVE_ERROR_COLOR DOODLE_ERROR_COLOR
#define NATIVE_SUFFIX_COLOR DOODLE_SUFFIX_COLOR

#ifdef NATIVE_LOG_ENABLED
#define NATIVE_LOG(...)                                                               \
    printf(NATIVE_COLOR NATIVE_MSG_PREFIX "[%s:%d] " NATIVE_SUFFIX_COLOR __VA_ARGS__, \
           DOODLE_FILENAME, DOODLE_LINE)

#define NATIVE_LOG_ERROR(...)                                                                      \
    printf(NATIVE_ERROR_COLOR NATIVE_MSG_PREFIX "[%s:%d][ERROR] " NATIVE_SUFFIX_COLOR __VA_ARGS__, \
           DOODLE_FILENAME, DOODLE_LINE)
#else
#define NATIVE_LOG(...) (void)0
#define NATIVE_LOG_ERROR(...) (void)0
#endif

#ifdef NATIVE_ALLOW_FATAL_ERRORS
#define NATIVE_FATAL_ERROR(...)        \
    do {                               \
        NATIVE_LOG_ERROR(__VA_ARGS__); \
        exit(1);                       \
    } while (0)
#else
#define NATIVE_FATAL_ERROR(...) NATIVE_LOG_ERROR(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __NATIVE_DEBUG_H__
