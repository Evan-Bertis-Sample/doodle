#ifndef __DOODLE_MATH_H__
#define __DOODLE_MATH_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef float doodle_real_t;

typedef doodle_real_t (*doodle_map_func_t)(doodle_real_t);

/**------------------------------------------------------------------------
 *                           doodle_vec2_t prototypes
 *------------------------------------------------------------------------**/

typedef struct doodle_vec2_t {
    union {
        struct {
            doodle_real_t x;
            doodle_real_t y;
        };
        doodle_real_t v[2];
    };
} doodle_vec2_t;

doodle_vec2_t doodle_vec2_make(doodle_real_t x, doodle_real_t y);
doodle_vec2_t doodle_vec2_is_nan(doodle_vec2_t v);
doodle_vec2_t doodle_vec2_add(doodle_vec2_t a, doodle_vec2_t b);
doodle_vec2_t doodle_vec2_sub(doodle_vec2_t a, doodle_vec2_t b);
doodle_vec2_t doodle_vec2_mul(doodle_vec2_t a, doodle_vec2_t b);
doodle_vec2_t doodle_vec2_div(doodle_vec2_t a, doodle_vec2_t b);
doodle_vec2_t doodle_vec2_scale(doodle_vec2_t v, doodle_real_t s);
doodle_real_t doodle_vec2_dot(doodle_vec2_t a, doodle_vec2_t b);
doodle_real_t doodle_vec2_cross(doodle_vec2_t a, doodle_vec2_t b);
doodle_real_t doodle_vec2_length(doodle_vec2_t v);
doodle_vec2_t doodle_vec2_normalize(doodle_vec2_t v);
doodle_vec2_t doodle_vec2_rotate(doodle_vec2_t v, doodle_real_t angle);
doodle_vec2_t doodle_vec2_reflect(doodle_vec2_t v, doodle_vec2_t normal);
doodle_vec2_t doodle_vec2_lerp(doodle_vec2_t a, doodle_vec2_t b, doodle_real_t t);
doodle_vec2_t doodle_vec2_min(doodle_vec2_t a, doodle_vec2_t b);
doodle_vec2_t doodle_vec2_max(doodle_vec2_t a, doodle_vec2_t b);
doodle_vec2_t doodle_vec2_clamp(doodle_vec2_t v, doodle_vec2_t min, doodle_vec2_t max);
doodle_vec2_t doodle_vec2_abs(doodle_vec2_t v);
doodle_vec2_t doodle_vec2_floor(doodle_vec2_t v);
doodle_vec2_t doodle_vec2_ceil(doodle_vec2_t v);
doodle_vec2_t doodle_vec2_map(doodle_vec2_t v, doodle_map_func_t func);

/**------------------------------------------------------------------------
 *                           doodle_vec3_t prototypes
 *------------------------------------------------------------------------**/

typedef struct doodle_vec3_t {
    union {
        struct {
            doodle_real_t x;
            doodle_real_t y;
            doodle_real_t z;
        };
        doodle_real_t v[3];
    };
} doodle_vec3_t;

/**------------------------------------------------------------------------
 *                           doodle_vec4_t prototypes
 *------------------------------------------------------------------------**/

doodle_vec3_t doodle_vec3_make(doodle_real_t x, doodle_real_t y, doodle_real_t z);
doodle_vec3_t doodle_vec3_is_nan(doodle_vec3_t v);
doodle_vec3_t doodle_vec3_add(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_sub(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_mul(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_div(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_scale(doodle_vec3_t v, doodle_real_t s);
doodle_real_t doodle_vec3_dot(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_cross(doodle_vec3_t a, doodle_vec3_t b);
doodle_real_t doodle_vec3_length(doodle_vec3_t v);
doodle_vec3_t doodle_vec3_normalize(doodle_vec3_t v);
doodle_vec3_t doodle_vec3_rotate(doodle_vec3_t v, doodle_vec3_t axis, doodle_real_t angle);
doodle_vec3_t doodle_vec3_reflect(doodle_vec3_t v, doodle_vec3_t normal);
doodle_vec3_t doodle_vec3_lerp(doodle_vec3_t a, doodle_vec3_t b, doodle_real_t t);
doodle_vec3_t doodle_vec3_min(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_max(doodle_vec3_t a, doodle_vec3_t b);
doodle_vec3_t doodle_vec3_clamp(doodle_vec3_t v, doodle_vec3_t min, doodle_vec3_t max);
doodle_vec3_t doodle_vec3_abs(doodle_vec3_t v);
doodle_vec3_t doodle_vec3_floor(doodle_vec3_t v);
doodle_vec3_t doodle_vec3_ceil(doodle_vec3_t v);
doodle_vec3_t doodle_vec3_map(doodle_vec3_t v, doodle_map_func_t func);

typedef struct doodle_vec4_t {
    union {
        struct {
            doodle_real_t x;
            doodle_real_t y;
            doodle_real_t z;
            doodle_real_t w;
        };
        doodle_real_t v[4];
    };
} doodle_vec4_t;

doodle_vec4_t doodle_vec4_make(doodle_real_t x, doodle_real_t y, doodle_real_t z, doodle_real_t w);
doodle_vec4_t doodle_vec4_is_nan(doodle_vec4_t v);
doodle_vec4_t doodle_vec4_add(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_sub(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_mul(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_div(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_scale(doodle_vec4_t v, doodle_real_t s);
doodle_real_t doodle_vec4_dot(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_cross(doodle_vec4_t a, doodle_vec4_t b);
doodle_real_t doodle_vec4_length(doodle_vec4_t v);
doodle_vec4_t doodle_vec4_normalize(doodle_vec4_t v);
doodle_vec4_t doodle_vec4_rotate(doodle_vec4_t v, doodle_vec4_t axis, doodle_real_t angle);
doodle_vec4_t doodle_vec4_reflect(doodle_vec4_t v, doodle_vec4_t normal);
doodle_vec4_t doodle_vec4_lerp(doodle_vec4_t a, doodle_vec4_t b, doodle_real_t t);
doodle_vec4_t doodle_vec4_min(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_max(doodle_vec4_t a, doodle_vec4_t b);
doodle_vec4_t doodle_vec4_clamp(doodle_vec4_t v, doodle_vec4_t min, doodle_vec4_t max);
doodle_vec4_t doodle_vec4_abs(doodle_vec4_t v);
doodle_vec4_t doodle_vec4_floor(doodle_vec4_t v);
doodle_vec4_t doodle_vec4_ceil(doodle_vec4_t v);
doodle_vec4_t doodle_vec4_map(doodle_vec4_t v, doodle_map_func_t func);

/**------------------------------------------------------------------------
 *                           doodle_vec conversions
 *------------------------------------------------------------------------**/

doodle_vec2_t doodle_vec2_from_vec3(doodle_vec3_t v);
doodle_vec2_t doodle_vec2_from_vec4(doodle_vec4_t v);

doodle_vec3_t doodle_vec3_from_vec2(doodle_vec2_t v);
doodle_vec3_t doodle_vec3_from_vec4(doodle_vec4_t v);

doodle_vec4_t doodle_vec4_from_vec2(doodle_vec2_t v);
doodle_vec4_t doodle_vec4_from_vec3(doodle_vec3_t v);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __DOODLE_MATH_H__