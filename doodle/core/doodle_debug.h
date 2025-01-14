#ifndef __DOODLE_DEBUG_H__
#define __DOODLE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>   // for printf()
#include <stdlib.h>  // for exit()
#include <string.h>  // for strrchr()

// Example toggles
#define DOODLE_CORE_LOG_ENABLED
#define DOODLE_CORE_ALLOW_FATAL_ERRORS  // if defined, will call exit() on fatal errors
#define DOODLE_APP_LOG_ENABLED
#define DOODLE_APP_ALLOW_FATAL_ERRORS

#define DOODLE_STR_HELPER(x) #x
#define DOODLE_STR(x) DOODLE_STR_HELPER(x)

#ifdef __clang__
// Clang-specific: __FILE_NAME__ is already the stripped filename
#define DOODLE_FILENAME __FILE_NAME__
#elif defined(__GNUC__)
// Evaluate at runtime (technically, it can be constant-folded):
#define DOODLE_FILENAME                         \
    (__builtin_strrchr(__FILE__, '/')           \
         ? __builtin_strrchr(__FILE__, '/') + 1 \
         : __FILE__)

#else
// Fallback for other compilers
#define DOODLE_FILENAME __FILE__
#endif

// If you still want a macro for the line:
#define DOODLE_LINE __LINE__

// Some color codes for console printing
#define DOODLE_PREFIX_COLOR "\033[1;34m"
#define DOODLE_ERROR_COLOR "\033[1;31m"
#define DOODLE_PLATFORM_COLOR "\033[1;32m"
#define DOODLE_APP_COLOR "\033[1;33m"
#define DOODLE_SUFFIX_COLOR "\033[0m"

// Just two prefix strings (no path/line in them)
// We'll insert [%s:%d] at runtime in the macros
#define DOODLE_CORE_MSG_PREFIX "[doodle-core]"
#define DOODLE_APP_MSG_PREFIX "[doodle-app]"

#ifdef DOODLE_CORE_LOG_ENABLED

// Example:
// [doodle-core] [myfile.c:42] some message
#define DOODLE_CORE_LOG(fmt, ...)                                                         \
    printf(DOODLE_PREFIX_COLOR DOODLE_CORE_MSG_PREFIX "[%s:%d] " DOODLE_SUFFIX_COLOR fmt, \
           DOODLE_FILENAME, DOODLE_LINE, ##__VA_ARGS__)

#define DOODLE_CORE_LOG_ERROR(fmt, ...)                                                              \
    printf(DOODLE_ERROR_COLOR DOODLE_CORE_MSG_PREFIX "[%s:%d][ERROR] " DOODLE_SUFFIX_COLOR fmt, \
           DOODLE_FILENAME, DOODLE_LINE, ##__VA_ARGS__)
#else
#define DOODLE_CORE_LOG(...) (void)0
#define DOODLE_CORE_LOG_ERROR(...) (void)0
#endif

#ifdef DOODLE_CORE_ALLOW_FATAL_ERRORS
#define DOODLE_CORE_FATAL_ERROR(...)        \
    {                                       \
        DOODLE_CORE_LOG_ERROR(__VA_ARGS__); \
        exit(1);                            \
    }
#else
#define DOODLE_CORE_FATAL_ERROR(...) DOODLE_CORE_LOG_ERROR(__VA_ARGS__)
#endif

#ifdef DOODLE_APP_LOG_ENABLED

// Example:
// [doodle-app][myfile.c:42] some message
#define DOODLE_APP_LOG(fmt, ...)                                                      \
    printf(DOODLE_APP_COLOR DOODLE_APP_MSG_PREFIX "[%s:%d] " DOODLE_SUFFIX_COLOR fmt, \
           DOODLE_FILENAME, DOODLE_LINE, ##__VA_ARGS__)

#define DOODLE_APP_LOG_ERROR(fmt, ...)                                                \
    printf(DOODLE_APP_COLOR DOODLE_APP_MSG_PREFIX "[%s:%d] " DOODLE_SUFFIX_COLOR fmt, \
           DOODLE_FILENAME, DOODLE_LINE, ##__VA_ARGS__)

#else
#define DOODLE_APP_LOG(...) (void)0
#define DOODLE_APP_LOG_ERROR(...) (void)0
#endif

#ifdef DOODLE_APP_ALLOW_FATAL_ERRORS
#define DOODLE_APP_FATAL_ERROR(...)        \
    {                                      \
        DOODLE_APP_LOG_ERROR(__VA_ARGS__); \
        exit(1);                           \
    }
#else
#define DOODLE_APP_FATAL_ERROR(...) DOODLE_APP_LOG_ERROR(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __DOODLE_DEBUG_H__
