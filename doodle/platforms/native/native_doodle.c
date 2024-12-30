/**------------------------------------------------------------------------
 *                             native_doodle.c
 *
 *  Implementation of the doodle platform interface for the native platform,
 *  using imgui for rendering.
 *
 *------------------------------------------------------------------------**/

#include <doodle/core/doodle_platform.h>

// platform-specific includes
#include <doodle/platforms/native/native_debug.h>
#include <doodle/platforms/native/native_input.h>
#include <doodle/platforms/native/native_renderer.h>

static void native_attach_exit_interrupts(void *ctx, void (*exit_handler)(void *)) {
    // no-op
}

doodle_platform_t doodle_platform_create(void **module_configs) {
    NATIVE_LOG("Module configs at %p\n", module_configs);

    for (uint32_t i = 0; i < DOODLE_MODULE_TYPE_COUNT; i++) {
        NATIVE_LOG("Module config %d: %p\n", i, module_configs[i]);
    }

    doodle_platform_t platform = {0};
    platform.attach_exit_interrupts = native_attach_exit_interrupts;

    NATIVE_LOG("Creating platform\n");

    NATIVE_LOG("Adding renderer module\n");
    void *renderer_config = module_configs[DOODLE_MODULE_TYPE_RENDERER];
    doodle_module_renderer_t *renderer = native_renderer_create(
        *(doodle_module_renderer_config_t *)renderer_config);

    doodle_platform_add_module(
        &platform,
        DOODLE_MODULE_TYPE_RENDERER,
        (doodle_module_t *)renderer);

    NATIVE_LOG("Adding input module\n");
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
