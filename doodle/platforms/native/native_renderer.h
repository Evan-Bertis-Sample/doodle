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

typedef struct native_renderer_ctx {
    doodle_module_renderer_config_t config;
    // some stuff regarding imgui...
    // ...
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
#endif // __cplusplus

#endif // __NATIVE_RENDERER_H__