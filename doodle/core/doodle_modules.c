/**------------------------------------------------------------------------
 *                             doodle_modules.c
 *  
 *  Functions for creating and managing doodle modules.
 *  
 *  
 *------------------------------------------------------------------------**/


#include <doodle/core/doodle_modules.h>

doodle_module_t doodle_module_create(doodle_module_type_t type, doodle_module_context_t context, void (*destroy)(void *module)) {
    doodle_module_t module = {
        .type = type,
        .context = context,
        .destroy = destroy,
        .checksum = 0,
    };

    // calculate the checksum
    module.checksum = 0;
    uint8_t *ptr = (uint8_t *)&module;
    for (uint32_t i = 0; i < sizeof(doodle_module_t); i++) {
        module.checksum += ptr[i];
    }

    return module;
}


bool doodle_module_verify(void *module) {
    doodle_module_t *mod = (doodle_module_t *)module;
    uint8_t checksum = 0;
    uint8_t *ptr = (uint8_t *)mod;
    for (uint32_t i = 0; i < sizeof(doodle_module_t); i++) {
        checksum += ptr[i];
    }

    return checksum == mod->checksum;
}