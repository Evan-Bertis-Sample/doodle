/**------------------------------------------------------------------------
 *                             native_renderer.c
 *  
 *  An implementation of the doodle renderer module for the native platform.
 *  Uses imgui for rendering.
 *  
 *------------------------------------------------------------------------**/

#include <doodle/platforms/native/native_renderer.h>
#include <stdlib.h>

static void __native_renderer_destroy(void *module) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)module;
    // some imgui cleanup stuff...
    // ...
    free(ctx);
}

static void __native_renderer_clear(doodle_module_renderer_t *renderer, doodle_color_t color) {
    // some imgui stuff...
    // ...
}

static void __native_renderer_draw_pixel(doodle_module_renderer_t *renderer, uint32_t x, uint32_t y, doodle_color_t color) {
    // some imgui stuff...
    // ...
}

static void __native_renderer_draw_line(doodle_module_renderer_t *renderer, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, doodle_color_t color) {
    // some imgui stuff...
    // ...
}

static void __native_renderer_draw_rect(doodle_module_renderer_t *renderer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, doodle_color_t color) {
    // some imgui stuff...
    // ...
}

static void __native_renderer_draw_circle(doodle_module_renderer_t *renderer, uint32_t x, uint32_t y, uint32_t radius, doodle_color_t color) {
    // some imgui stuff...
    // ...
}

static void __native_renderer_draw_text(doodle_module_renderer_t *renderer, uint32_t x, uint32_t y, const char *text, doodle_color_t color) {
    // some imgui stuff...
    // ...
}


doodle_module_renderer_t *native_renderer_create(doodle_module_renderer_config_t config) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)malloc(sizeof(native_renderer_ctx_t));
    if (!ctx) {
        return NULL;
    }

    ctx->config = config;
    // some imgui initialization stuff...
    // ...

    doodle_module_renderer_t *renderer = (doodle_module_renderer_t *)malloc(sizeof(doodle_module_renderer_t));
    if (!renderer) {
        free(ctx);
        return NULL;
    }

    doodle_module_t module = doodle_module_create(DOODLE_MODULE_TYPE_RENDERER, ctx, __native_renderer_destroy);

    *renderer = (doodle_module_renderer_t){
        .module = module,
        .config = config,
        .clear = __native_renderer_clear,
        .draw_pixel = __native_renderer_draw_pixel,
        .draw_line = __native_renderer_draw_line,
        .draw_rect = __native_renderer_draw_rect,
        .draw_circle = __native_renderer_draw_circle,
        .draw_text = __native_renderer_draw_text,
    };

    printf("Renderer created\n");

    return renderer;
}
