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

    ctx->dirty_regions[ctx->dirty_count++] = rect;
}

static void __native_renderer_destroy(void *module) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)module;
    // If you had per-renderer ImGui resources (custom fonts, textures, etc.),
    // you could free them here. The global ImGui context is managed by shared_gui.
    free(ctx);
}

static void __native_renderer_clear(doodle_module_renderer_t *renderer, doodle_color_t color) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
    // memset() is faster than a loop for filling a buffer with a single value.
    memset(ctx->offscreen.pixels, color.value, ctx->offscreen.width * ctx->offscreen.height * sizeof(doodle_color_t));
}

static void __native_renderer_draw_pixel(doodle_module_renderer_t *renderer, uint32_t x, uint32_t y, doodle_color_t color) {
    native_renderer_ctx_t *ctx = (native_renderer_ctx_t *)renderer->module.context;
    if (x >= ctx->config.width || y >= ctx->config.height) {
        return;  // Out of bounds
    }

    // offscreen buffer is row-major: row y, column x
    ctx->offscreen.pixels[y * ctx->config.width + x] = color;

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
}

static void __native_renderer_draw_circle(doodle_module_renderer_t *renderer,
                                          uint32_t x, uint32_t y,
                                          uint32_t radius,
                                          doodle_color_t color) {
    ImDrawList *draw_list = igGetBackgroundDrawList_Nil();
    ImU32 col32 = IM_COL32(color.r, color.g, color.b, color.a);

    float fx = (float)x;
    float fy = (float)y;
    float fr = (float)radius;

    // Filled circle. For just an outline, use ImDrawList_AddCircle().
    // The 16 parameter is the number of segments. Increase if you want smoother circles.
    ImDrawList_AddCircleFilled(draw_list, (ImVec2){fx, fy}, fr, col32, 16);
}

static void __native_renderer_draw_text(doodle_module_renderer_t *renderer,
                                        uint32_t x, uint32_t y,
                                        const char *text,
                                        doodle_color_t color) {
    if (!text) {
        return;  // Nothing to draw
    }

    ImDrawList *draw_list = igGetBackgroundDrawList_Nil();
    ImU32 col32 = IM_COL32(color.r, color.g, color.b, color.a);

    float fx = (float)x;
    float fy = (float)y;

    // Draw text using ImGui’s default font. For custom fonts, you’d load them in shared_gui and set them.
    // Note: ImGui draws text anchored from the top-left corner of the text.
    // Adjust as needed for your coordinate system.
    ImDrawList_AddText_Vec2(draw_list, (ImVec2){fx, fy}, col32, text, NULL);
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
    };

    NATIVE_LOG("Native renderer created\n");
    return renderer;
}
