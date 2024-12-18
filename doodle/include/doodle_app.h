#ifndef __DOODLE_APP_H__
#define __DOODLE_APP_H__

#include "doodle_math.h"
#include "doodle_platform.h"

typedef struct doodle_app_desc {
    const char *name;
    const doodle_vec2_t display_size;
    void (*setup)(void);
    void (*loop)(void);
    void (*teardown)(void);
} doodle_app_desc_t;

typedef struct doodle_app {
    doodle_app_desc_t desc;
    doodle_platform_t platform;
} doodle_app_t;

doodle_app_t doodle_app_create(doodle_app_desc_t desc, doodle_platform_t platform);

void doodle_app_init(doodle_app_t *app);
void doodle_app_loop(doodle_app_t *app);
void doodle_app_teardown(doodle_app_t *app);

// global variable for the app
doodle_app_t g_doodle_app;

#ifdef DOODLE_MAIN
doodle_app_desc_t doodle_main(int32_t argc, char *argv[]);

int main(int32_t argc, char *argv[]);

#endif  // DOODLE_MAIN

#endif  // __DOODLE_APP_H__