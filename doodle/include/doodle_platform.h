#ifndef __DOODLE_PLATFORM_H__
#define __DOODLE_PLATFORM_H__

typedef void* doodle_platform_context_t;

typedef struct doodle_platform {
    doodle_platform_context_t context;
    void (*init)(doodle_platform_context_t context);
    void (*shutdown)(doodle_platform_context_t context);
    void (*update)(doodle_platform_context_t context);

    void (*attach_exit_interrupts)(doodle_platform_context_t context, void (*exit_handler)(void*));
} doodle_platform_t;

// These functions should be implemented in the platform-specific code.
// They will be left blank here.
// If the linker complains about missing symbols, you need to implement them.
doodle_platform_t doodle_platform_create();
void doodle_platform_destroy(doodle_platform_t platform);

#endif  // __DOODLE_PLATFORM_H__