/**------------------------------------------------------------------------
 *                             native_input.c
 *  
 *  An implementation of the doodle input module for the native platform.
 *  Uses imgui to mock input.
 *  
 *------------------------------------------------------------------------**/

#include <doodle/platforms/native/native_input.h>
#include <doodle/platforms/native/native_debug.h>

#include <stdlib.h>

static void __native_input_destroy(void *module) {
    native_input_ctx_t *ctx = (native_input_ctx_t *)module;
    // some imgui cleanup stuff...
    // ...
    free(ctx);
}

doodle_module_input_t *native_input_create(doodle_module_input_config_t config) {
    native_input_ctx_t *ctx = (native_input_ctx_t *)malloc(sizeof(native_input_ctx_t));
    if (!ctx) {
        NATIVE_FATAL_ERROR("Failed to allocate memory for native input\n");
        return NULL;
    }

    ctx->config = config;
    // some imgui initialization stuff...
    // ...

    doodle_module_input_t *input = (doodle_module_input_t *)malloc(sizeof(doodle_module_input_t));
    if (!input) {
        free(ctx);
        NATIVE_FATAL_ERROR("Failed to allocate memory for native input\n");
        return NULL;
    }

    doodle_module_t module = doodle_module_create(DOODLE_MODULE_TYPE_INPUT, ctx, __native_input_destroy);
    if (!doodle_module_verify(&module)) {
        free(ctx);
        free(input);
        return NULL;
    }

    *input = (doodle_module_input_t){
        .module = module,
        .config = config,
        .update = NULL, // TODO: implement this
    };

    return input;
}