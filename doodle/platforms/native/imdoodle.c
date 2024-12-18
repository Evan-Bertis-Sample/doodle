/**------------------------------------------------------------------------
 *                             imdoodle.c
 *  
 *  Implementation of the doodle platform interface for the native platform,
 *  using imgui for rendering.
 *  
 *------------------------------------------------------------------------**/

#include "doodle_platform.h"
#include "platforms/native/imdoodle.h"

doodle_platform_t doodle_platform_create()
{
    doodle_platform_t platform = {0};
    return platform;
}

void doodle_platform_destroy(doodle_platform_t platform)
{
    // no-op
}

