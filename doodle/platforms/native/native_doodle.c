/**------------------------------------------------------------------------
 *                             native_doodle.c
 *
 *  Implementation of the doodle platform interface for the native platform,
 *  using imgui for rendering.
 *
 *------------------------------------------------------------------------**/

#include <doodle/core/doodle_platform.h>

// platform-specific includes
#include <doodle/platforms/native/native_renderer.h>
#include <doodle/platforms/native/native_input.h>


static void native_attach_exit_interrupts(void * ctx, void (*exit_handler)(void *)) {
    // no-op
}

doodle_platform_t doodle_platform_create(void **module_configs) {
    doodle_platform_t platform = {0};
    platform.attach_exit_interrupts = native_attach_exit_interrupts;

    printf("Creating platform\n");

    doodle_platform_add_module(
        &platform,
        DOODLE_MODULE_TYPE_RENDERER,
        (doodle_module_t *)native_renderer_create(
            *(doodle_module_renderer_config_t *)(module_configs[DOODLE_MODULE_TYPE_RENDERER])));


    doodle_platform_add_module(
        &platform,
        DOODLE_MODULE_TYPE_INPUT,
        (doodle_module_t *)native_input_create(
            *(doodle_module_input_config_t *)(module_configs[DOODLE_MODULE_TYPE_INPUT])));

    printf("Platform created\n");

    return platform;
}

void doodle_platform_destroy(doodle_platform_t platform) {
    // no-op
}
