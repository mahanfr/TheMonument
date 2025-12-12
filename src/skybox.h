#ifndef _SKYBOX_HEADER
#define _SKYBOX_HEADER
#pragma once

#include "raylib.h"

typedef Model Skybox;
Skybox skybox_init(const char* cubemap);
void skybox_render(Skybox skybox);
void skybox_distroy(Skybox skybox);

#endif
