/**------------------------------------------------------------------------
 *                             native_doodle.c
 *
 *  Implementation of the doodle platform interface for the native platform,
 *  using imgui for rendering.
 *
 *------------------------------------------------------------------------**/

#include <doodle/core/doodle_platform.h>

static void native_attach_exit_interrupts(doodle_platform_context_t context, void (*exit_handler)(void *)) {
    // no-op
}

doodle_platform_t doodle_platform_create() {
    doodle_platform_t platform = {0};
    platform.attach_exit_interrupts = native_attach_exit_interrupts;

    return platform;
}

void doodle_platform_destroy(doodle_platform_t platform) {
    // no-op
}
