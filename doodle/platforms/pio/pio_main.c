/**------------------------------------------------------------------------
 *                           pio/main.c
 *
 * This defines the main function for arduino sketches using the doodle framework.
 *
 *------------------------------------------------------------------------**/

#include <Arduino.h>

#include <doodle/core/doodle_app.h>
#include <doodle/core/doodle_platform.h>

doodle_app_t g_doodle_app;

static void exit_handler(void *ctx) {
    doodle_app_teardown(&g_doodle_app);
    doodle_platform_t platform = g_doodle_app.platform;
    platform.shutdown(platform.context);
}

void setup() {
    doodle_app_desc_t desc = doodle_main(0, 0);
    doodle_platform_t platform = doodle_platform_create(desc.module_configs);
    g_doodle_app = doodle_app_create(desc, platform);
    doodle_app_init(&g_doodle_app);
    // attach a few interrupt handlers to handle application quitting
    // and other things
    g_doodle_app.platform.attach_exit_interrupts(g_doodle_app.platform.context, exit_handler);
}

void loop() {
    doodle_app_loop(&g_doodle_app);
}