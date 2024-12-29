#ifndef __NATIVE_INPUT_H__
#define __NATIVE_INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <doodle/core/doodle_modules.h>
#include <doodle/core/modules/doodle_input.h>

typedef struct native_input_ctx {
    doodle_module_input_config_t config;
    // some stuff regarding input...
    // ...
} native_input_ctx_t;


/**------------------------------------------------------------------------
 *                           Native Input Functions
 *------------------------------------------------------------------------**/

/// @brief Creates a new native input module using IMGUI.
/// @param config The configuration for the input module.
/// @return A new native input module.
doodle_module_input_t *native_input_create(doodle_module_input_config_t config);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __NATIVE_INPUT_H__