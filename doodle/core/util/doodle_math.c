#include <doodle/core/util/doodle_math.h>

#include <float.h>
#include <math.h>

/**------------------------------------------------------------------------
 *                           doodle_vec2_t functions
 *------------------------------------------------------------------------**/

doodle_vec2_t doodle_vec2_make(doodle_real_t x, doodle_real_t y) {
    doodle_vec2_t v;
    v.x = x;
    v.y = y;
    return v;
}

doodle_vec2_t doodle_vec2_is_nan(doodle_vec2_t v) {
    doodle_vec2_t result;
    result.x = isnan(v.x) ? 1.0f : 0.0f;
    result.y = isnan(v.y) ? 1.0f : 0.0f;
    return result;
}

doodle_vec2_t doodle_vec2_add(doodle_vec2_t a, doodle_vec2_t b) {
    return doodle_vec2_make(a.x + b.x, a.y + b.y);
}

doodle_vec2_t doodle_vec2_sub(doodle_vec2_t a, doodle_vec2_t b) {
    return doodle_vec2_make(a.x - b.x, a.y - b.y);
}

doodle_vec2_t doodle_vec2_mul(doodle_vec2_t a, doodle_vec2_t b) {
    return doodle_vec2_make(a.x * b.x, a.y * b.y);
}

doodle_vec2_t doodle_vec2_div(doodle_vec2_t a, doodle_vec2_t b) {
    return doodle_vec2_make(a.x / b.x, a.y / b.y);
}

doodle_vec2_t doodle_vec2_scale(doodle_vec2_t v, doodle_real_t s) {
    return doodle_vec2_make(v.x * s, v.y * s);
}

doodle_real_t doodle_vec2_dot(doodle_vec2_t a, doodle_vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

doodle_real_t doodle_vec2_cross(doodle_vec2_t a, doodle_vec2_t b) {
    // In 2D, the cross product can be represented as a scalar: a.x*b.y - a.y*b.x
    return a.x * b.y - a.y * b.x;
}

doodle_real_t doodle_vec2_length(doodle_vec2_t v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

doodle_vec2_t doodle_vec2_normalize(doodle_vec2_t v) {
    doodle_real_t len = doodle_vec2_length(v);
    if (len > 0.0f) {
        return doodle_vec2_scale(v, 1.0f / len);
    }
    return doodle_vec2_make(0.0f, 0.0f);
}

doodle_vec2_t doodle_vec2_rotate(doodle_vec2_t v, doodle_real_t angle) {
    doodle_real_t c = cosf(angle);
    doodle_real_t s = sinf(angle);
    return doodle_vec2_make(c * v.x - s * v.y, s * v.x + c * v.y);
}

doodle_vec2_t doodle_vec2_reflect(doodle_vec2_t v, doodle_vec2_t normal) {
    // reflection: v - 2*(v·n)*n
    doodle_real_t d = doodle_vec2_dot(v, normal);
    return doodle_vec2_sub(v, doodle_vec2_scale(normal, 2.0f * d));
}

doodle_vec2_t doodle_vec2_lerp(doodle_vec2_t a, doodle_vec2_t b, doodle_real_t t) {
    return doodle_vec2_make(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

doodle_vec2_t doodle_vec2_min(doodle_vec2_t a, doodle_vec2_t b) {
    return doodle_vec2_make(fminf(a.x, b.x), fminf(a.y, b.y));
}

doodle_vec2_t doodle_vec2_max(doodle_vec2_t a, doodle_vec2_t b) {
    return doodle_vec2_make(fmaxf(a.x, b.x), fmaxf(a.y, b.y));
}

doodle_vec2_t doodle_vec2_clamp(doodle_vec2_t v, doodle_vec2_t min, doodle_vec2_t max) {
    return doodle_vec2_make(
        fminf(fmaxf(v.x, min.x), max.x),
        fminf(fmaxf(v.y, min.y), max.y));
}

doodle_vec2_t doodle_vec2_abs(doodle_vec2_t v) {
    return doodle_vec2_make(fabsf(v.x), fabsf(v.y));
}

doodle_vec2_t doodle_vec2_floor(doodle_vec2_t v) {
    return doodle_vec2_make(floorf(v.x), floorf(v.y));
}

doodle_vec2_t doodle_vec2_ceil(doodle_vec2_t v) {
    return doodle_vec2_make(ceilf(v.x), ceilf(v.y));
}

doodle_vec2_t doodle_vec2_map(doodle_vec2_t v, doodle_map_func_t func) {
    return doodle_vec2_make(func(v.x), func(v.y));
}


/**------------------------------------------------------------------------
 *                           doodle_vec3_t functions
 *------------------------------------------------------------------------**/


doodle_vec3_t doodle_vec3_make(doodle_real_t x, doodle_real_t y, doodle_real_t z) {
    doodle_vec3_t v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

doodle_vec3_t doodle_vec3_is_nan(doodle_vec3_t v) {
    doodle_vec3_t result;
    result.x = isnan(v.x) ? 1.0f : 0.0f;
    result.y = isnan(v.y) ? 1.0f : 0.0f;
    result.z = isnan(v.z) ? 1.0f : 0.0f;
    return result;
}

doodle_vec3_t doodle_vec3_add(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(a.x + b.x, a.y + b.y, a.z + b.z);
}

doodle_vec3_t doodle_vec3_sub(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(a.x - b.x, a.y - b.y, a.z - b.z);
}

doodle_vec3_t doodle_vec3_mul(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(a.x * b.x, a.y * b.y, a.z * b.z);
}

doodle_vec3_t doodle_vec3_div(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(a.x / b.x, a.y / b.y, a.z / b.z);
}

doodle_vec3_t doodle_vec3_scale(doodle_vec3_t v, doodle_real_t s) {
    return doodle_vec3_make(v.x * s, v.y * s, v.z * s);
}

doodle_real_t doodle_vec3_dot(doodle_vec3_t a, doodle_vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

doodle_vec3_t doodle_vec3_cross(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

doodle_real_t doodle_vec3_length(doodle_vec3_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

doodle_vec3_t doodle_vec3_normalize(doodle_vec3_t v) {
    doodle_real_t len = doodle_vec3_length(v);
    if (len > 0.0f) {
        return doodle_vec3_scale(v, 1.0f / len);
    }
    return doodle_vec3_make(0.0f, 0.0f, 0.0f);
}

doodle_vec3_t doodle_vec3_rotate(doodle_vec3_t v, doodle_vec3_t axis, doodle_real_t angle) {
    // Rodrigues' rotation formula:
    // v_rot = v*cos(angle) + (axis x v)*sin(angle) + axis*(axis·v)*(1 - cos(angle))
    doodle_vec3_t u = doodle_vec3_normalize(axis);
    doodle_real_t c = cosf(angle);
    doodle_real_t s = sinf(angle);

    doodle_vec3_t v_proj = doodle_vec3_scale(u, doodle_vec3_dot(u, v));
    doodle_vec3_t v_perp = doodle_vec3_sub(v, v_proj);
    doodle_vec3_t w = doodle_vec3_cross(u, v);

    doodle_vec3_t term1 = doodle_vec3_scale(v, c);
    doodle_vec3_t term2 = doodle_vec3_scale(w, s);
    doodle_vec3_t term3 = doodle_vec3_scale(u, doodle_vec3_dot(u, v) * (1.0f - c));

    return doodle_vec3_add(doodle_vec3_add(term1, term2), term3);
}

doodle_vec3_t doodle_vec3_reflect(doodle_vec3_t v, doodle_vec3_t normal) {
    doodle_real_t d = doodle_vec3_dot(v, normal);
    return doodle_vec3_sub(v, doodle_vec3_scale(normal, 2.0f * d));
}

doodle_vec3_t doodle_vec3_lerp(doodle_vec3_t a, doodle_vec3_t b, doodle_real_t t) {
    return doodle_vec3_make(a.x + (b.x - a.x) * t,
                            a.y + (b.y - a.y) * t,
                            a.z + (b.z - a.z) * t);
}

doodle_vec3_t doodle_vec3_min(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
}

doodle_vec3_t doodle_vec3_max(doodle_vec3_t a, doodle_vec3_t b) {
    return doodle_vec3_make(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
}

doodle_vec3_t doodle_vec3_clamp(doodle_vec3_t v, doodle_vec3_t min, doodle_vec3_t max) {
    return doodle_vec3_make(
        fminf(fmaxf(v.x, min.x), max.x),
        fminf(fmaxf(v.y, min.y), max.y),
        fminf(fmaxf(v.z, min.z), max.z));
}

doodle_vec3_t doodle_vec3_abs(doodle_vec3_t v) {
    return doodle_vec3_make(fabsf(v.x), fabsf(v.y), fabsf(v.z));
}

doodle_vec3_t doodle_vec3_floor(doodle_vec3_t v) {
    return doodle_vec3_make(floorf(v.x), floorf(v.y), floorf(v.z));
}

doodle_vec3_t doodle_vec3_ceil(doodle_vec3_t v) {
    return doodle_vec3_make(ceilf(v.x), ceilf(v.y), ceilf(v.z));
}

doodle_vec3_t doodle_vec3_map(doodle_vec3_t v, doodle_map_func_t func) {
    return doodle_vec3_make(func(v.x), func(v.y), func(v.z));
}

// Again, conversion functions have suspicious prototypes. Implementing as no-ops.
doodle_vec3_t doodle_vec3_to_vec2(doodle_vec3_t v) {
    return v;  // Should return a vec2 logically, but prototype returns a vec3.
}

doodle_vec3_t doodle_vec3_to_vec4(doodle_vec3_t v) {
    return v;  // Should return a vec4 logically, but prototype returns a vec3.
}

/**------------------------------------------------------------------------
 *                           doodle_vec4_t functions
 *------------------------------------------------------------------------**/

doodle_vec4_t doodle_vec4_make(doodle_real_t x, doodle_real_t y, doodle_real_t z, doodle_real_t w) {
    doodle_vec4_t v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}

doodle_vec4_t doodle_vec4_is_nan(doodle_vec4_t v) {
    doodle_vec4_t result;
    result.x = isnan(v.x) ? 1.0f : 0.0f;
    result.y = isnan(v.y) ? 1.0f : 0.0f;
    result.z = isnan(v.z) ? 1.0f : 0.0f;
    result.w = isnan(v.w) ? 1.0f : 0.0f;
    return result;
}

doodle_vec4_t doodle_vec4_add(doodle_vec4_t a, doodle_vec4_t b) {
    return doodle_vec4_make(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

doodle_vec4_t doodle_vec4_sub(doodle_vec4_t a, doodle_vec4_t b) {
    return doodle_vec4_make(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

doodle_vec4_t doodle_vec4_mul(doodle_vec4_t a, doodle_vec4_t b) {
    return doodle_vec4_make(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

doodle_vec4_t doodle_vec4_div(doodle_vec4_t a, doodle_vec4_t b) {
    return doodle_vec4_make(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

doodle_vec4_t doodle_vec4_scale(doodle_vec4_t v, doodle_real_t s) {
    return doodle_vec4_make(v.x * s, v.y * s, v.z * s, v.w * s);
}

doodle_real_t doodle_vec4_dot(doodle_vec4_t a, doodle_vec4_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

doodle_vec4_t doodle_vec4_cross(doodle_vec4_t a, doodle_vec4_t b) {
    // For 4D vectors, a standard cross product isn't well-defined.
    // We might treat it as just the cross product of the xyz parts and w = 0.
    doodle_real_t cx = a.y * b.z - a.z * b.y;
    doodle_real_t cy = a.z * b.x - a.x * b.z;
    doodle_real_t cz = a.x * b.y - a.y * b.x;
    return doodle_vec4_make(cx, cy, cz, 0.0f);
}

doodle_real_t doodle_vec4_length(doodle_vec4_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

doodle_vec4_t doodle_vec4_normalize(doodle_vec4_t v) {
    doodle_real_t len = doodle_vec4_length(v);
    if (len > 0.0f) {
        return doodle_vec4_scale(v, 1.0f / len);
    }
    return doodle_vec4_make(0.0f, 0.0f, 0.0f, 0.0f);
}

doodle_vec4_t doodle_vec4_rotate(doodle_vec4_t v, doodle_vec4_t axis, doodle_real_t angle) {
    // 4D rotation is not defined without specifying a plane of rotation.
    // Since the prototype is given without clarification, return v unchanged.
    (void)axis;
    (void)angle;
    return v;
}

doodle_vec4_t doodle_vec4_reflect(doodle_vec4_t v, doodle_vec4_t normal) {
    doodle_real_t d = doodle_vec4_dot(v, normal);
    return doodle_vec4_sub(v, doodle_vec4_scale(normal, 2.0f * d));
}

doodle_vec4_t doodle_vec4_lerp(doodle_vec4_t a, doodle_vec4_t b, doodle_real_t t) {
    return doodle_vec4_make(a.x + (b.x - a.x) * t,
                            a.y + (b.y - a.y) * t,
                            a.z + (b.z - a.z) * t,
                            a.w + (b.w - a.w) * t);
}

doodle_vec4_t doodle_vec4_min(doodle_vec4_t a, doodle_vec4_t b) {
    return doodle_vec4_make(
        fminf(a.x, b.x),
        fminf(a.y, b.y),
        fminf(a.z, b.z),
        fminf(a.w, b.w));
}

doodle_vec4_t doodle_vec4_max(doodle_vec4_t a, doodle_vec4_t b) {
    return doodle_vec4_make(
        fmaxf(a.x, b.x),
        fmaxf(a.y, b.y),
        fmaxf(a.z, b.z),
        fmaxf(a.w, b.w));
}

doodle_vec4_t doodle_vec4_clamp(doodle_vec4_t v, doodle_vec4_t min, doodle_vec4_t max) {
    return doodle_vec4_make(
        fminf(fmaxf(v.x, min.x), max.x),
        fminf(fmaxf(v.y, min.y), max.y),
        fminf(fmaxf(v.z, min.z), max.z),
        fminf(fmaxf(v.w, min.w), max.w));
}

doodle_vec4_t doodle_vec4_abs(doodle_vec4_t v) {
    return doodle_vec4_make(fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w));
}

doodle_vec4_t doodle_vec4_floor(doodle_vec4_t v) {
    return doodle_vec4_make(floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w));
}

doodle_vec4_t doodle_vec4_ceil(doodle_vec4_t v) {
    return doodle_vec4_make(ceilf(v.x), ceilf(v.y), ceilf(v.z), ceilf(v.w));
}

doodle_vec4_t doodle_vec4_map(doodle_vec4_t v, doodle_map_func_t func) {
    return doodle_vec4_make(func(v.x), func(v.y), func(v.z), func(v.w));
}


/**------------------------------------------------------------------------
 *                           doodle vec conversions
 *------------------------------------------------------------------------**/

doodle_vec2_t doodle_vec2_from_vec3(doodle_vec3_t v) {
    return doodle_vec2_make(v.x, v.y);
}

doodle_vec2_t doodle_vec2_from_vec4(doodle_vec4_t v) {
    return doodle_vec2_make(v.x, v.y);
}

doodle_vec3_t doodle_vec3_from_vec2(doodle_vec2_t v) {
    return doodle_vec3_make(v.x, v.y, 0.0f);
}

doodle_vec3_t doodle_vec3_from_vec4(doodle_vec4_t v) {
    return doodle_vec3_make(v.x, v.y, v.z);
}

doodle_vec4_t doodle_vec4_from_vec2(doodle_vec2_t v) {
    return doodle_vec4_make(v.x, v.y, 0.0f, 0.0f);
}

doodle_vec4_t doodle_vec4_from_vec3(doodle_vec3_t v) {
    return doodle_vec4_make(v.x, v.y, v.z, 0.0f);
}