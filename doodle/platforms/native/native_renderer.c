#include <doodle/core/modules/doodle_renderer.h>
#include <doodle/platforms/native/native_debug.h>
#include <doodle/platforms/native/native_gui.h>
#include <doodle/platforms/native/native_renderer.h>
#include <stdlib.h>

static void __native_renderer_mark_dirty(native_renderer_ctx_t *ctx, doodle_rect_t rect) {
    if (ctx->dirty_count >= MAX_DIRTY_REGIONS) {
        NATIVE_LOG_ERROR("Exceeded maximum dirty regions\n");
        return;
    }

    // check if the region is already dirty, i.e if it is already fully contained in another dirty region
    for (uint32_t i = 0; i < ctx->dirty_count; i++) {
        if (doodle_rect_fully_contains(ctx->dirty_regions[i], rect)) {
            return;  // Already dirty
        }

        if (doodle_rect_fully_contains(rect, ctx->dirty_regions[i])) {
            // Replace the dirty region with the new region
            ctx->dirty_regions[i] = rect;
            return;
        }
    }

    ctx->dirty_regions[ctx->dirty_count++] = rect;
}

static void __native_renderer_clear(doodle_module_renderer_t *renderer, doodle_color_t color) {
    NATIVE_LOG("Clearing the screen\n");
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
    // memset() is faster than a loop for filling a buffer with a single value.
    memset(ctx->offscreen.pixels, color.value, ctx->offscreen.width * ctx->offscreen.height * sizeof(doodle_color_t));
    __native_renderer_mark_dirty(ctx, (doodle_rect_t){0, 0, ctx->config.width, ctx->config.height});
}

static void __native_renderer_draw_pixel(doodle_module_renderer_t *renderer, uint32_t x, uint32_t y, doodle_color_t color) {
    NATIVE_LOG("Drawing pixel at: %d, %d\n", x, y);
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
    if (x >= ctx->config.width || y >= ctx->config.height) {
        NATIVE_LOG("Draw pixel out of bounds: %d, %d\n", x, y);
        return;  // Out of bounds
    }

    // offscreen buffer is row-major: row y, column x
    doodle_texture_set_pixel(&ctx->offscreen, x, y, color);

    // Mark the region as dirty
    doodle_rect_t dirty_rect = {
        .x = x,
        .y = y,
        .width = 1,
        .height = 1,
    };

    __native_renderer_mark_dirty(ctx, dirty_rect);
}

static void __native_renderer_draw_line(doodle_module_renderer_t *renderer,
                                        uint32_t x0, uint32_t y0,
                                        uint32_t x1, uint32_t y1,
                                        doodle_color_t color) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
}

static void __native_renderer_draw_rect(doodle_module_renderer_t *renderer,
                                        uint32_t x, uint32_t y,
                                        uint32_t width, uint32_t height,
                                        doodle_color_t color) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
    if (x >= ctx->config.width || y >= ctx->config.height) {
        NATIVE_LOG("Draw rect out of bounds: %d, %d\n", x, y);
        return;  // Out of bounds
    }

    // Draw the rectangle by drawing each pixel
    // so basically, we jump to the left side of the row, then memset the row to the width
    // then we jump the line down
    // kind of like a scanline
    for (uint32_t row = y; row < y + height; row++) {
        if (row >= ctx->config.height) {
            break;  // Out of bounds
        }

        // offscreen buffer is row-major: row y, column x
        memset(&ctx->offscreen.pixels[row * ctx->config.width + x], color.value, width * sizeof(doodle_color_t));
    }

    // Mark the region as dirty
    doodle_rect_t dirty_rect = {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };

    __native_renderer_mark_dirty(ctx, dirty_rect);
}

static void __native_renderer_draw_circle(doodle_module_renderer_t *renderer,
                                          uint32_t x, uint32_t y,
                                          uint32_t radius,
                                          doodle_color_t color) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;

    // Draw the circle by drawing each pixel
    // just like, draw it stupidly
    for (uint32_t row = y - radius; row < y + radius; row++) {
        if (row >= ctx->config.height) {
            NATIVE_LOG("Draw circle out of bounds: %d, %d\n", x, y);
            break;  // Out of bounds
        }

        for (uint32_t col = x - radius; col < x + radius; col++) {
            if (col >= ctx->config.width) {
                NATIVE_LOG("Draw circle out of bounds: %d, %d\n", x, y);
                break;  // Out of bounds
            }

            // offscreen buffer is row-major: row y, column x
            ctx->offscreen.pixels[row * ctx->config.width + col] = color;
        }
    }
}

static void __native_renderer_draw_text(doodle_module_renderer_t *renderer,
                                        uint32_t x, uint32_t y,
                                        const char *text,
                                        doodle_color_t color) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
    NATIVE_LOG_ERROR("Draw text not implemented\n");
}

static void __native_renderer_blit(doodle_module_renderer_t *renderer, doodle_rect_t rect) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;

    for (uint32_t i = 0; i < ctx->dirty_count; i++) {
        doodle_rect_t dirty_rect = ctx->dirty_regions[i];
        // Blit the dirty region to the screen
        // This is where you would copy the offscreen buffer to the screen
        // using the platform's native graphics API (e.g., D3D11, OpenGL, etc.)
        NATIVE_LOG("Blitting dirty region: %d, %d, %d, %d\n",
                   dirty_rect.x, dirty_rect.y, dirty_rect.width, dirty_rect.height);
    }

    ctx->dirty_count = 0;  // Reset the dirty region count
}

//--------------------------------------------------------------------------------------
// Destroy Function
//--------------------------------------------------------------------------------------

static void __native_renderer_destroy(void *module) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)module;
    doodle_texture_destroy(ctx->offscreen);
    free(ctx);
}

//--------------------------------------------------------------------------------------
// Create Function
//--------------------------------------------------------------------------------------
doodle_module_renderer_t *native_renderer_create(doodle_module_renderer_config_t config) {
    NATIVE_LOG("Creating native renderer\n");

    // If you haven't already, ensure the shared_gui is initialized somewhere in your program.
    // e.g., in your main() or a platform init function:
    //   native_gui_initialize("Hello World!", config.width, config.height);

    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)malloc(sizeof(native_renderer_ctx_t));
    if (!ctx) {
        NATIVE_FATAL_ERROR("Failed to allocate memory for native renderer\n");
        return NULL;
    }

    ctx->config = config;  // Save the config so you can access width/height if needed

    doodle_module_renderer_t *renderer = (doodle_module_renderer_t *)malloc(sizeof(doodle_module_renderer_t));
    if (!renderer) {
        NATIVE_FATAL_ERROR("Failed to allocate memory for native renderer\n");
        free(ctx);
        return NULL;
    }

    NATIVE_LOG("Creating native renderer module\n");

    // Create a doodle_module_t wrapper that calls our destroy function when done
    doodle_module_t module = doodle_module_create(DOODLE_MODULE_TYPE_RENDERER, ctx, __native_renderer_destroy);

    module.context = malloc(sizeof(native_renderer_ctx_t));
    native_renderer_ctx_t *module_ctx = (native_renderer_ctx_t *)module.context;
    if (!module_ctx) {
        NATIVE_FATAL_ERROR("Failed to allocate memory for native renderer context\n");
        free(renderer);
        free(ctx);
        return NULL;
    }

    // Initialize the offscreen buffer
    module_ctx->offscreen = doodle_texture_create(config.width, config.height);

    if (native_gui_initialize("doodle", config.width, config.height)) {
        NATIVE_LOG("Native GUI initialized\n");
    } else {
        NATIVE_FATAL_ERROR("Failed to initialize native GUI\n");
        free(renderer);
        free(ctx);
        return NULL;
    }

    // Fill out the function pointers
    *renderer = (doodle_module_renderer_t){
        .module = module,
        .config = config,
        .clear = __native_renderer_clear,
        .draw_pixel = __native_renderer_draw_pixel,
        .draw_line = __native_renderer_draw_line,
        .draw_rect = __native_renderer_draw_rect,
        .draw_circle = __native_renderer_draw_circle,
        .draw_text = __native_renderer_draw_text,
        .blit = __native_renderer_blit,
    };

    NATIVE_LOG("Native renderer created\n");
    return renderer;
}