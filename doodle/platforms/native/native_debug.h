#ifndef __NATIVE_DEBUG_H__
#define __NATIVE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NATIVE_LOG_ENABLED
#define NATIVE_MSG_PREFIX "[doodle-platform][native]"

#ifdef NATIVE_LOG_ENABLED
#define NATIVE_LOG(...) printf(NATIVE_MSG_PREFIX " " __VA_ARGS__)
#define NATIVE_LOG_ERROR(...) printf(NATIVE_MSG_PREFIX " [ERROR] " __VA_ARGS__)
#else
#define NATIVE_LOG(...) void(0)
#define NATIVE_LOG_ERROR(...) void(0)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __NATIVE_DEBUG_H__