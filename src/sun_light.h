#ifndef _SUNLIGHT_HEADER
#define _SUNLIGHT_HEADER

#include "raylib.h"

typedef struct {
    Shader shader;
} SunLight;

SunLight sunlight_init(Color ambient, Color light, float shininess);
void sunlight_apply(SunLight sun, Model models[], int count);
void sunlight_set_direction(SunLight sun, Vector3 new_direction);
void sunlight_update(SunLight sun, Vector3 camera_position);
void sunlight_distroy(SunLight sun);
#endif
