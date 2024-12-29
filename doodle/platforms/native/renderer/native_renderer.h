#ifndef __NATIVE_RENDERER_H__
#define __NATIVE_RENDERER_H__

#include <doodle/core/doodle_modules.h>
#include <doodle/core/modules/doodle_renderer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct native_renderer_ctx {
    // some stuff regarding imgui...
    // ...
} native_renderer_ctx_t;


/**------------------------------------------------------------------------
 *                           Native Renderer Functions
 *------------------------------------------------------------------------**/

/// @brief Creates a new native renderer module, but does not initialize it.
/// @param config 
/// @return A new native renderer module.
doodle_module_renderer_t *native_renderer_interface_create(void *config);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __NATIVE_RENDERER_H__