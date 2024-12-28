/**------------------------------------------------------------------------
 *                             doodle_app.c
 *
 *  Defines the main application structure and functions.
 *  Also contains the "main" function that initializes the application, if
 *  you'd like to use it.
 *
 *------------------------------------------------------------------------**/

#include <doodle/core/doodle_app.h>
#include <doodle/core/doodle_platform.h>

doodle_app_t doodle_app_create(doodle_app_desc_t desc, doodle_platform_t platform) {
    doodle_app_t app = {
        .desc = desc,
        .platform = platform,
    };
    return app;
}

void doodle_app_init(doodle_app_t *app) {
    app->desc.setup(app);
}

void doodle_app_teardown(doodle_app_t *app) {
    app->desc.teardown(app);
}

void doodle_app_loop(doodle_app_t *app) {
    app->desc.loop(app);
}