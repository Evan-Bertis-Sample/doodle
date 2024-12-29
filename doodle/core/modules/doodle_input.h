#ifndef __DOODLE_INPUT_H__
#define __DOODLE_INPUT_H__

#include <stdint.h>

#include <doodle/core/doodle_modules.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum doodle_input_type {
    DOODLE_INPUT_TYPE_NONE,
    DOODLE_INPUT_TYPE_BOOLEAN,     // for things like buttons
    DOODLE_INPUT_TYPE_CONTINUOUS,  // for things like sensors
    DOODLE_INPUT_TYPE_EVENT,       // for things like touch screens/interrupts
    DOODLE_INPUT_COUNT,            // for counting the number of types
} doodle_input_type_t;

typedef struct doodle_input {
    doodle_input_type_t type;
    void *(*read)(void);
} doodle_input_t;

typedef struct doodle_module_input_config {
    doodle_input_t *inputs;
    uint32_t input_count;
} doodle_module_input_config_t;

typedef struct doodle_module_input {
    doodle_module_t module;
    doodle_module_input_config_t config;

    void (*update)(struct doodle_module_input *input);
} doodle_module_input_t;


#ifdef __cplusplus
}
#endif  // __cplusplus


#endif  // __DOODLE_INPUT_H__