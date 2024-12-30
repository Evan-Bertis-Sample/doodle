/**------------------------------------------------------------------------
 *                             main.c
 *
 *  This defines the main function for a standard native platform.
 *  It will assume that you have implemented the doodle_main function in
 *  your application, which will return a doodle_app_desc_t.
 *
 *------------------------------------------------------------------------**/


#include <doodle/core/doodle_app.h>
#include <doodle/core/doodle_platform.h>
#include <doodle/platforms/native/native_debug.h>

static void exit_handler(void *ctx) {
    doodle_app_t *app = (doodle_app_t *)ctx;
    doodle_app_teardown(app);
    doodle_platform_t platform = app->platform;
    platform.shutdown(platform.context);
}

int main(int32_t argc, char *argv[]) {
    NATIVE_LOG("Invoking main\n");
    doodle_app_desc_t desc = doodle_main(argc, argv);

    doodle_platform_t platform = doodle_platform_create(desc.module_configs);

    NATIVE_LOG("Creating app\n");
    doodle_app_t app = doodle_app_create(desc, platform);

    NATIVE_LOG("Attaching exit interrupts\n");
    // attach a few interrupt handlers to handle application quitting
    // and other things
    app.platform.attach_exit_interrupts((void *)&app, exit_handler);

    NATIVE_LOG("Initializing app\n");
    doodle_app_init(&app);

    while (1) {
        doodle_app_loop(&app);
    }
}