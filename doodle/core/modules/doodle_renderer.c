#include <doodle/core/modules/doodle_renderer.h>
#include <doodle/core/doodle_debug.h>

#include <stdlib.h>

doodle_texture_t doodle_texture_create(uint32_t width, uint32_t height) {
    doodle_texture_t texture = {
        .width = width,
        .height = height,
        .pixels = (doodle_color_t *)malloc(width * height * sizeof(doodle_color_t)),
    };

    if (!texture.pixels) {
        DOODLE_CORE_LOG_ERROR("Failed to allocate texture pixels\n");
        return texture;
    }

    // Clear the texture to black
    for (uint32_t i = 0; i < width * height; i++) {
        texture.pixels[i].value = 0xFF000000;
    }

    return texture;
}

void doodle_texture_destroy(doodle_texture_t texture) {
    free(texture.pixels);
}

uint32_t doodle_texture_get_idx(doodle_texture_t *texture, uint32_t x, uint32_t y) {
    return y * texture->width + x;
}

void doodle_texture_set_pixel(doodle_texture_t *texture, uint32_t x, uint32_t y, doodle_color_t color) {
    if (x >= texture->width || y >= texture->height) {
        DOODLE_CORE_LOG_ERROR("Set pixel out of bounds: %d, %d\n", x, y);
        return;
    }

    texture->pixels[doodle_texture_get_idx(texture, x, y)] = color;
}

doodle_color_t doodle_texture_get_pixel(doodle_texture_t *texture, uint32_t x, uint32_t y) {
    if (x >= texture->width || y >= texture->height) {
        DOODLE_CORE_LOG_ERROR("Get pixel out of bounds: %d, %d\n", x, y);
        return (doodle_color_t){0};
    }

    return texture->pixels[doodle_texture_get_idx(texture, x, y)];
}

doodle_rect_t doodle_rect_convert_to_corner(doodle_rect_t rect) {
    if (rect.mode == DOODLE_RECTMODE_CORNER) {
        return rect;
    }

    return (doodle_rect_t){
        .mode = DOODLE_RECTMODE_CORNER,
        .x = rect.x - rect.width / 2,
        .y = rect.y - rect.height / 2,
        .width = rect.width,
        .height = rect.height,
    };
}

doodle_rect_t doodle_rect_convert_to_center(doodle_rect_t rect) {
    if (rect.mode == DOODLE_RECTMODE_CENTER) {
        return rect;
    }

    return (doodle_rect_t){
        .mode = DOODLE_RECTMODE_CENTER,
        .x = rect.x + rect.width / 2,
        .y = rect.y + rect.height / 2,
        .width = rect.width,
        .height = rect.height,
    };
}

bool doodle_rect_contains(doodle_rect_t rect, uint32_t x, uint32_t y) {
    rect = doodle_rect_convert_to_corner(rect);

    return x >= rect.x && x < rect.x + rect.width && y >= rect.y && y < rect.y + rect.height;
}

bool doodle_rect_intersects(doodle_rect_t a, doodle_rect_t b) {
    a = doodle_rect_convert_to_corner(a);
    b = doodle_rect_convert_to_corner(b);

    return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

bool doodle_rect_fully_contains(doodle_rect_t a, doodle_rect_t b) {
    a = doodle_rect_convert_to_corner(a);
    b = doodle_rect_convert_to_corner(b);

    return a.x <= b.x && a.x + a.width >= b.x + b.width && a.y <= b.y && a.y + a.height >= b.y + b.height;
}

doodle_rect_t doodle_rect_combine(doodle_rect_t a, doodle_rect_t b) {
    a = doodle_rect_convert_to_corner(a);
    b = doodle_rect_convert_to_corner(b);

    uint32_t x = a.x < b.x ? a.x : b.x;
    uint32_t y = a.y < b.y ? a.y : b.y;
    uint32_t width = (a.x + a.width > b.x + b.width ? a.x + a.width : b.x + b.width) - x;
    uint32_t height = (a.y + a.height > b.y + b.height ? a.y + a.height : b.y + b.height) - y;

    return (doodle_rect_t){
        .mode = DOODLE_RECTMODE_CORNER,
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };
}