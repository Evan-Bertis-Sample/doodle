#ifndef __DOODLE_PLATFORM_H__
#define __DOODLE_PLATFORM_H__

#include <doodle/core/doodle_modules.h>

typedef void* doodle_platform_context_t;

typedef struct doodle_platform {
    doodle_platform_context_t context;
    doodle_module_t *modules[DOODLE_MODULE_TYPE_COUNT];

    void (*init)(doodle_platform_context_t context);
    void (*shutdown)(doodle_platform_context_t context);
    void (*update)(doodle_platform_context_t context);
    void (*attach_exit_interrupts)(doodle_platform_context_t context, void (*exit_handler)(void*));
} doodle_platform_t;


/**------------------------------------------------------------------------
 *                           doodle_platform functions!
 * In order to create a platform, you must implement these two functions, at least.
 *------------------------------------------------------------------------**/



/// @brief Creates a new doodle platform. By default, this is not implemented my the doodle core.
/// @note You must implement this function in your platform-specific code.
/// @note At a minimum, you should attach the doole_module_t modules to the platform, so that they can be used.
/// @return A new doodle_platform_t object.
doodle_platform_t doodle_platform_create();
void doodle_platform_destroy(doodle_platform_t platform);
void doodle_platform_add_module(doodle_platform_t *platform, doodle_module_type_t type, doodle_module_t *module);

#endif  // __DOODLE_PLATFORM_H__