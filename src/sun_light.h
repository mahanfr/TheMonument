#ifndef SUNLIGHT_HEADER
#define SUNLIGHT_HEADER

#include "raylib.h"
#include "game.h"

typedef struct SunLight SunLight;

SunLight *sunlight_init(Color ambient, Color light, float shininess);
void sunlight_apply(Game *game);
void sunlight_set_direction(SunLight *sun, Vector3 new_direction);
void sunlight_update(SunLight *sun, Vector3 camera_position);
void sunlight_distroy(SunLight *sun);
#endif
