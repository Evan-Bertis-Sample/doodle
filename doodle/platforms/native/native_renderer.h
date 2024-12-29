#ifndef __NATIVE_RENDERER_H__
#define __NATIVE_RENDERER_H__

#include <doodle/core/doodle_modules.h>
#include <doodle/core/modules/doodle_renderer.h>

#ifdef __cplusplus
extern "C" {
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