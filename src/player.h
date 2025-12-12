#ifndef _PLAYER_HEADER
#define _PLAYER_HEADER
#pragma once
#include "raylib.h"

typedef struct {
    long id;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Model model;
} Player;

#endif
