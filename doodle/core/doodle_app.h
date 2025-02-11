#ifndef __DOODLE_APP_H__
#define __DOODLE_APP_H__

#include <doodle/core/util/doodle_math.h>
#include <doodle/core/doodle_platform.h>
#include <doodle/core/doodle_modules.h>

#ifdef __cplusplus
extern "C" {
#endif

// forward declaration
typedef struct doodle_app doodle_app_t;

typedef struct doodle_app_desc {
    char *name;
    void *module_configs[DOODLE_MODULE_TYPE_COUNT];
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

void *doodle_app_get_module(doodle_app_t *app, doodle_module_type_t type);

// This is a specific implementation, which will describe the main function
doodle_app_desc_t doodle_main(int32_t argc, char *argv[]);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __DOODLE_APP_H__