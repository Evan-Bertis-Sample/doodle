/**------------------------------------------------------------------------
 *                             doodle_modules.c
 *  
 *  Functions for creating and managing doodle modules.
 *  
 *  
 *------------------------------------------------------------------------**/


#include <doodle/core/doodle_debug.h>
#include <doodle/core/doodle_modules.h>

// lol
#define CHECKSUM 0x00d00d1e

doodle_module_t doodle_module_create(doodle_module_type_t type, doodle_module_context_t context, void (*destroy)(void *module)) {
    doodle_module_t module = {
        .type = type,
        .context = context,
        .destroy = destroy,
        .checksum = CHECKSUM,
    };
    return module;
}


bool doodle_module_verify(void *module) {
    doodle_module_t mod = *(doodle_module_t *)module;

    // DOODLE_CORE_LOG("Verifying module\n");
    // DOODLE_CORE_LOG("Module type: %d\n", mod.type);
    // DOODLE_CORE_LOG("Module context: %p\n", mod.context);
    // DOODLE_CORE_LOG("Module destroy: %p\n", mod.destroy);
    // DOODLE_CORE_LOG("Module checksum: %d\n", mod.checksum);

    return mod.checksum == CHECKSUM;
}