#ifndef __NATIVE_RENDERER_H__
#define __NATIVE_RENDERER_H__

#include <doodle/core/doodle_modules.h>
#include <doodle/core/modules/doodle_renderer.h>

#ifdef __cplusplus
extern "C" {
#endif

// this has some cpp symbols, so we need to wrap it in extern "C"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui/cimgui.h>

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS
#define WIN32_LEAN_AND_MEAN
#include <GLFW/glfw3.h>
#include <d3d11.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

#define MAX_DIRTY_REGIONS 64

typedef struct native_renderer_ctx {
    doodle_module_renderer_config_t config;

    // The offscreen buffer we draw into for each command.
    doodle_texture_t offscreen;

    // The texture we display on screen.
    // Because we are mocking an adafruit display,
    // what happens is we go through each dirty rect, then redo the full region
    // display that, then do that
    doodle_texture_t onscreen;

    // List of dirty rectangles that need to be updated on screen at next blit.
    doodle_rect_t dirty_regions[MAX_DIRTY_REGIONS];
    size_t dirty_count;
} native_renderer_ctx_t;

/**------------------------------------------------------------------------
 *                           Native Renderer Functions
 *------------------------------------------------------------------------**/

/// @brief Creates a new native renderer module using IMGUI.
/// @param config The configuration for the renderer.
/// @return A new native renderer module.
doodle_module_renderer_t *native_renderer_create(doodle_module_renderer_config_t config);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __NATIVE_RENDERER_H__