#ifndef __DOODLE_DEBUG_H__
#define __DOODLE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h> // for exit()
#include <stdio.h>  // for printf()

#define DOODLE_CORE_LOG_ENABLED
#define DOODLE_CORE_ALLOW_FATAL_ERRORS // if defined, will call exit() on fatal errors
#define DOODLE_APP_LOG_ENABLED
#define DOODLE_APP_ALLOW_FATAL_ERRORS


#ifdef __clang__
#define DOODLE_FILENAME __FILE_NAME__
#elif __GNUC__
#define DOODLE_FILENAME __builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__
#else
#define DOODLE_FILENAME __FILE__
#endif

#define DOODLE_STR_HELPER(x) #x
#define DOODLE_STR(x) DOODLE_STR_HELPER(x)
#define DOODLE_LINE __LINE__

#define DOODLE_PREFIX_COLOR "\033[1;34m"
#define DOODLE_ERROR_COLOR "\033[1;31m"
#define DOODLE_PLATFORM_COLOR "\033[1;32m"
#define DOODLE_APP_COLOR "\033[1;33m"
#define DOODLE_SUFFIX_COLOR "\033[0m"

#define DOODLE_CORE_MSG_PREFIX "[doodle-core]" "[" DOODLE_FILENAME ":" DOODLE_STR(DOODLE_LINE) "]"
#define DOODLE_APP_MSG_PREFIX "[doodle-app]" "[" DOODLE_FILENAME ":" DOODLE_STR(DOODLE_LINE) "]"

#ifdef DOODLE_CORE_LOG_ENABLED
#define DOODLE_CORE_LOG(...) printf(DOODLE_PREFIX_COLOR DOODLE_CORE_MSG_PREFIX " " DOODLE_SUFFIX_COLOR __VA_ARGS__)
#define DOODLE_CORE_LOG_ERROR(...) printf(DOODLE_ERROR_COLOR DOODLE_CORE_MSG_PREFIX " [ERROR] " DOODLE_SUFFIX_COLOR __VA_ARGS__)
#else
#define DOODLE_CORE_LOG(...) void(0)
#define DOODLE_CORE_LOG_ERROR(...) void(0)
#endif

#ifdef DOODLE_CORE_ALLOW_FATAL_ERRORS
#define DOODLE_CORE_FATAL_ERROR(...) { DOODLE_CORE_LOG_ERROR(__VA_ARGS__); exit(1); }
#else
#define DOODLE_CORE_FATAL_ERROR(...) DOODLE_CORE_LOG_ERROR(__VA_ARGS__)
#endif

#ifdef DOODLE_APP_LOG_ENABLED
#define DOODLE_APP_LOG(...) printf(DOODLE_APP_COLOR DOODLE_APP_MSG_PREFIX " " DOODLE_SUFFIX_COLOR __VA_ARGS__)
#define DOODLE_APP_LOG_ERROR(...) printf(DOODLE_ERROR_COLOR DOODLE_APP_MSG_PREFIX " [ERROR] " DOODLE_SUFFIX_COLOR __VA_ARGS__)
#else
#define DOODLE_APP_LOG(...) void(0)
#define DOODLE_APP_LOG_ERROR(...) void(0)
#endif

#ifdef DOODLE_APP_ALLOW_FATAL_ERRORS
#define DOODLE_APP_FATAL_ERROR(...) { DOODLE_APP_LOG_ERROR(__VA_ARGS__); exit(1); }
#else
#define DOODLE_APP_FATAL_ERROR(...) DOODLE_APP_LOG_ERROR(__VA_ARGS__)
#endif


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __DOODLE_DEBUG_H__