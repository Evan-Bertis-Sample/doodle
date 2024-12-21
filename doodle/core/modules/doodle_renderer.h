#ifndef __DOODLE_DISPLAY_H__
#define __DOODLE_DISPLAY_H__

#include <doodle_modules.h>
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
} doodle_color_t;

typedef struct doodle_module_renderer_config {
    uint32_t width;
    uint32_t height;
} doodle_module_renderer_config_t;

typedef struct doodle_module_renderer {
    doodle_module_t module;
    doodle_module_renderer_config_t config;

    void (*clear)(struct doodle_module_renderer *renderer, doodle_color_t color);
    void (*draw_pixel)(struct doodle_module_renderer *renderer, uint32_t x, uint32_t y, doodle_color_t color);
    void (*draw_line)(struct doodle_module_renderer *renderer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, doodle_color_t color);
    void (*draw_rect)(struct doodle_module_renderer *renderer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, doodle_color_t color);
    void (*draw_circle)(struct doodle_module_renderer *renderer, uint32_t x, uint32_t y, uint32_t radius, doodle_color_t color);
    void (*draw_text)(struct doodle_module_renderer *renderer, uint32_t x, uint32_t y, const char *text, doodle_color_t color);
} doodle_module_renderer_t;

#endif  // __DOODLE_DISPLAY_H__