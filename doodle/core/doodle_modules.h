#ifndef __DOODLE_MODULES_H__
#define __DOODLE_MODULES_H__

typedef enum doodle_module_type {
    DOODLE_MODULE_TYPE_RENDERER,
    DOODLE_MODULE_TYPE_INPUT,
    DOODLE_MODULE_TYPE_AUDIO,
    DOODLE_MODULE_TYPE_FILESYSTEM,
    DOODLE_MODULE_TYPE_COUNT
} doodle_module_type_t;

typedef void* doodle_module_context_t;

// Used as a header for all modules.
// Must be the first member of any module struct.
typedef struct doodle_module {
    doodle_module_type_t type;
    doodle_module_context_t context;
    struct doodle_module *(*create)(void *config);
    void (*destroy)(struct doodle_module *module);
} doodle_module_t;


#endif // __DOODLE_MODULES_H__