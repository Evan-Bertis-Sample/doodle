/**------------------------------------------------------------------------
 *                             doodle_app.c
 *
 *  Defines the main application structure and functions.
 *  Also contains the "main" function that initializes the application, if
 *  you'd like to use it.
 *
 *------------------------------------------------------------------------**/

#include "doodle_app.h"

#include "doodle_platform.h"

doodle_app_t doodle_app_create(doodle_app_desc_t desc, doodle_platform_t platform) {
    doodle_app_t app = {
        .desc = desc,
        .platform = platform,
    };
    return app;
}

void doodle_app_init(doodle_app_t *app) {
    app->desc.setup();
}

void doodle_app_teardown(doodle_app_t *app) {
    app->desc.teardown();
}

void doodle_app_loop(doodle_app_t *app) {
    app->desc.loop();
}

#ifdef DOODLE_MAIN

static void exit_handler(void *ctx) {
    doodle_app_teardown(&g_doodle_app);
    doodle_platform_t platform = g_doodle_app.platform;
    platform.shutdown(platform.context);
}

int main(int32_t argc, char *argv[]) {
    doodle_app_desc_t desc = doodle_main(argc, argv);
    doodle_platform_t platform = doodle_platform_create(NULL);
    g_doodle_app = doodle_app_create(desc, platform);
    doodle_app_init(&app)
        // attach a few interrupt handlers to handle application quitting
        // and other things
        g_doodle_app.platform.attach_exit_interrupts(g_doodle_app.platform.context, exit_handler);

    while (1) {
        doodle_app_loop(&app);
    }
}

#endif  // DOODLE_MAIN