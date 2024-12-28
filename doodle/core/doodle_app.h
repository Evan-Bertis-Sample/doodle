#ifndef __DOODLE_APP_H__
#define __DOODLE_APP_H__

#include <doodle/core/util/doodle_math.h>
#include <doodle/core/doodle_platform.h>

// forward declaration
typedef struct doodle_app doodle_app_t;

typedef struct doodle_app_desc {
    char *name;
    doodle_vec2_t display_size;
    void (*setup)(doodle_app_t *app);
    void (*loop)(doodle_app_t *app);
    void (*teardown)(doodle_app_t *app);
} doodle_app_desc_t;

typedef struct doodle_app {
    doodle_app_desc_t desc;
    doodle_platform_t platform;
} doodle_app_t;

doodle_app_t doodle_app_create(doodle_app_desc_t desc, doodle_platform_t platform);

void doodle_app_init(doodle_app_t *app);
void doodle_app_loop(doodle_app_t *app);
void doodle_app_teardown(doodle_app_t *app);

// This is a specific implementation, which will describe the main function
doodle_app_desc_t doodle_main(int32_t argc, char *argv[]);


#endif  // __DOODLE_APP_H__