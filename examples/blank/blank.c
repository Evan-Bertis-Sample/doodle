#include <stdio.h>

#include <doodle_math.h>

int main() {
    doodle_vec4_t vec = doodle_vec4_make(1.0f, 2.0f, 3.0f, 4.0f);
    printf("Hello, World!\n");

    printf("vec: %f %f %f %f\n", vec.x, vec.y, vec.z, vec.w);
    return 0;
}