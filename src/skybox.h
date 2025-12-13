#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER

#include "raylib.h"

typedef Model Skybox;
Skybox *skybox_init(const char* cubemap);
void skybox_render(Skybox *skybox);
void skybox_distroy(Skybox *skybox);

#endif
