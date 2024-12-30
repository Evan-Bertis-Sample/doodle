#ifndef __DOODLE_DEBUG_H__
#define __DOODLE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h> // for exit()
#include <stdio.h>  // for printf()

#define DOODLE_LOG_ENABLED
#define DOODLE_ALLOW_FATAL_ERRORS // if defined, will call exit() on fatal errors

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

#define DOODLE_CORE_MSG_PREFIX "[doodle-core]" "[" DOODLE_FILENAME ":" DOODLE_STR(DOODLE_LINE) "]"

#ifdef DOODLE_LOG_ENABLED
#define DOODLE_CORE_LOG(...) printf(DOODLE_CORE_MSG_PREFIX " " __VA_ARGS__)
#define DOODLE_CORE_LOG_ERROR(...) printf(DOODLE_CORE_MSG_PREFIX " [ERROR] " __VA_ARGS__)
#else
#define DOODLE_CORE_LOG(...) void(0)
#define DOODLE_CORE_LOG_ERROR(...) void(0)
#endif

#ifdef DOODLE_ALLOW_FATAL_ERRORS
#define DOODLE_CORE_FATAL_ERROR(...) { DOODLE_CORE_LOG_ERROR(__VA_ARGS__); exit(1); }
#else
#define DOODLE_CORE_FATAL_ERROR(...) DOODLE_CORE_LOG_ERROR(__VA_ARGS__)
#endif


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __DOODLE_DEBUG_H__