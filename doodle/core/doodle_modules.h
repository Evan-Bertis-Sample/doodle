#ifndef __DOODLE_MODULES_H__
#define __DOODLE_MODULES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum doodle_module_type {
    DOODLE_MODULE_TYPE_NONE,        // For invalid or uninitialized modules
    DOODLE_MODULE_TYPE_INPUT,       // For simple devices like buttons, sensors, etc.
    DOODLE_MODULE_TYPE_OUTPUT,      // For simple devices like LEDs, motors, etc.
    DOODLE_MODULE_TYPE_PERIPHERAL,  // For more complex, custom devices like robots, drones, etc.
    DOODLE_MODULE_TYPE_RENDERER,    // For display devices like screens, projectors, etc.
    DOODLE_MODULE_TYPE_AUDIO,       // For audio devices like speakers, microphones, etc.
    DOODLE_MODULE_TYPE_FILESYSTEM,  // For file systems, storage devices, etc.
    DOODLE_MODULE_TYPE_COUNT        // For counting the number of types
} doodle_module_type_t;

typedef void *doodle_module_context_t;

// Used as a header for all modules.
// Must be the first member of any module struct.
typedef struct doodle_module {
    doodle_module_type_t type;
    doodle_module_context_t context;
    void (*destroy)(void *module);
    uint8_t checksum; // this is used to verify the module is valid
} doodle_module_t;

/// @brief Function that must be called when creating a new module. It creates a checksum for the module.
/// @param type The type of module to create.
/// @param context The context for the module.
/// @param destroy The function to call when the module is destroyed.
/// @return 
doodle_module_t doodle_module_create(doodle_module_type_t type, doodle_module_context_t context, void (*destroy)(void *module));

/// @brief Verifies that the module is valid.
/// @param module The module to verify. It is important that the first member of the module is the doodle_module_t struct.
/// @note This basically just checks the checksum of the module.
bool doodle_module_verify(void *module);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __DOODLE_MODULES_H__