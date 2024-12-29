/**------------------------------------------------------------------------
 *                             doodle_platform.c
 *
 *  Standard implementation of doodle platform functions.
 *
 *------------------------------------------------------------------------**/

#include <doodle/core/doodle_platform.h>

void doodle_platform_add_module(doodle_platform_t *platform, doodle_module_type_t type, doodle_module_t *module) {
    platform->modules[type] = module;
}

void *doodle_platform_get_module(doodle_platform_t *platform, doodle_module_type_t type) {
    return platform->modules[type];
}