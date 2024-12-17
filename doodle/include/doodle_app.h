#ifndef __DOODLE_APP_H__
#define __DOODLE_APP_H__

#include "doodle/include/doodle_math.h"

typedef struct doodle_app_desc {
    const char *name;
    const doodle_vec2_t display_size;
} doodle_app_desc_t;

typedef struct doodle_app {
    doodle_app_desc_t desc;
    void (*setup)(void);
    void (*loop)(void);
    void (*teardown)(void);
} doodle_app_t;

void doodle_app_init(doodle_app_t *app);
void doodle_app_loop(doodle_app_t *app);
void doodle_app_teardown(doodle_app_t *app);

#ifdef DOODLE_MAIN
doodle_app_desc_t doodle_main(int32_t argc, char *argv[]);
#endif  // DOODLE_MAIN

#endif  // __DOODLE_APP_H__