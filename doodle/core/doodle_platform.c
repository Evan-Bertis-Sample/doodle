/**------------------------------------------------------------------------
 *                             doodle_platform.c
 *
 *  Standard implementation of doodle platform functions.
 *
 *------------------------------------------------------------------------**/

#include <doodle/core/doodle_platform.h>
#include <doodle/core/doodle_debug.h>

void doodle_platform_add_module(doodle_platform_t *platform, doodle_module_type_t type, doodle_module_t *module) {
    // check if the module is valid
    if (!doodle_module_verify(module)) {
        DOODLE_CORE_FATAL_ERROR("Invalid module\n");
        return;
    }

    // check that the module is infact the correct type
    if (module->type != type) {
        DOODLE_CORE_FATAL_ERROR("Module type mismatch\n");
        return;
    }

    platform->modules[type] = module;
}

void *doodle_platform_get_module(doodle_platform_t *platform, doodle_module_type_t type) {
    return platform->modules[type];
}