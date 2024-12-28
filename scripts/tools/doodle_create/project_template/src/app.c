#include <stdio.h>

#include <doodle/core/doodle_app.h>
#include <doodle/core/util/doodle_math.h>

static void blank_setup(void) {
    printf("setup\n");
}

static void blank_loop(void) {
    printf("loop\n");
}

static void blank_teardown(void) {
    printf("teardown\n");
}

doodle_app_desc_t doodle_main(int32_t argc, char *argv[]) {
    doodle_app_desc_t desc = {
        .name = "blank",
        .display_size = doodle_vec2_make(800.0f, 600.0f),
        .setup = blank_setup,
        .loop = blank_loop,
        .teardown = blank_teardown,
    };
    return desc;
}   