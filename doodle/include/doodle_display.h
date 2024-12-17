#ifndef __DOODLE_DISPLAY_H__
#define __DOODLE_DISPLAY_H__

#include <stdint.h>

typedef uint8_t doodle_color_channel_t;

typedef struct doodle_color {
    union {
        struct {
            doodle_color_channel_t r;
            doodle_color_channel_t g;
            doodle_color_channel_t b;
            doodle_color_channel_t a;
        };
        uint32_t value;
    };
};

typedef struct doodle_display {
    void *context;

} doodle_display_t;

#endif  // __DOODLE_DISPLAY_H__