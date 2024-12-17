#ifndef __DOODLE_INPUT_H__
#define __DOODLE_INPUT_H__

typedef enum doodle_input_type {
    DOODLE_INPUT_TYPE_NONE,
    DOODLE_INPUT_TYPE_BOOLEAN,     // for things like buttons
    DOODLE_INPUT_TYPE_CONTINUOUS,  // for things like sensors
    DOODLE_INPUT_TYPE_EVENT,       // for things like touch screens/interrupts
    DOODLE_INPUT_COUNT,            // for counting the number of types
} doodle_input_type_t;

typedef doodle_input_t {
    doodle_input_type_t type;
    void *data;  // pointer to the data
}

#endif  // __DOODLE_INPUT_H__