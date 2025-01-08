#ifndef __DOODLE_DISPLAY_H__
#define __DOODLE_DISPLAY_H__

#include <doodle/core/doodle_modules.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct doodle_texture {
    uint32_t width;
    uint32_t height;
    doodle_color_t *pixels;
} doodle_texture_t;

typedef enum doodle_rectmode {
    DOODLE_RECTMODE_CORNER,
    DOODLE_RECTMODE_CENTER,
} doodle_rectmode_t;

typedef struct doodle_rect {
    doodle_rectmode_t mode;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} doodle_rect_t;

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
    void (*blit)(struct doodle_module_renderer *renderer, doodle_rect_t rect);
    void (*blit_texture)(struct doodle_module_renderer *renderer, doodle_texture_t *texture, doodle_rect_t rect);

} doodle_module_renderer_t;

#define DOODLE_RENDERER_FUNC(renderer, func, ...) \
    if (renderer && renderer->func) {             \
        renderer->func(renderer, __VA_ARGS__);    \
    }

#ifdef __cplusplus
}
#endif  // __cplusplus

#define DOODLE_COLOR_RGBA(r, g, b, a) ((doodle_color_t){r, g, b, a})

// predefined color macros
#define DOODLE_COLOR_BLACK DOODLE_COLOR_RGBA(0, 0, 0, 255)
#define DOODLE_COLOR_WHITE DOODLE_COLOR_RGBA(255, 255, 255, 255)
#define DOODLE_COLOR_RED DOODLE_COLOR_RGBA(255, 0, 0, 255)
#define DOODLE_COLOR_GREEN DOODLE_COLOR_RGBA(0, 255, 0, 255)
#define DOODLE_COLOR_BLUE DOODLE_COLOR_RGBA(0, 0, 255, 255)
#define DOODLE_COLOR_YELLOW DOODLE_COLOR_RGBA(255, 255, 0, 255)
#define DOODLE_COLOR_CYAN DOODLE_COLOR_RGBA(0, 255, 255, 255)
#define DOODLE_COLOR_MAGENTA DOODLE_COLOR_RGBA(255, 0, 255, 255)
#define DOODLE_COLOR_NORTHWESTERN_PURPLE DOODLE_COLOR_RGBA(78, 42, 132, 255)

#endif  // __DOODLE_DISPLAY_H__