/**------------------------------------------------------------------------
 *                             main.c
 *  
 *  This defines the main function for a standard native platform.
 *  It will assume that you have implemented the doodle_main function in
 *  your application, which will return a doodle_app_desc_t.
 *  
 *------------------------------------------------------------------------**/


#include <doodle_app.h>
#include <doodle_platform.h>

doodle_app_t g_doodle_app;

static void exit_handler(void *ctx) {
    doodle_app_teardown(&g_doodle_app);
    doodle_platform_t platform = g_doodle_app.platform;
    platform.shutdown(platform.context);
}

int main(int32_t argc, char *argv[]) {
    doodle_app_desc_t desc = doodle_main(argc, argv);
    doodle_platform_t platform = doodle_platform_create(NULL);
    g_doodle_app = doodle_app_create(desc, platform);
    doodle_app_init(&g_doodle_app);
        // attach a few interrupt handlers to handle application quitting
        // and other things
        g_doodle_app.platform.attach_exit_interrupts(g_doodle_app.platform.context, exit_handler);

    while (1) {
        doodle_app_loop(&g_doodle_app);
    }
}