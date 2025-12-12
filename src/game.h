#ifndef _GAME_HEADER
#define _GAME_HEADER
#pragma once
#include "raylib.h"
#include "skybox.h"
#include "sun_light.h"
#include "player.h"

typedef struct {} Light;

typedef struct {
    long id;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Model model;
} GameEntity;

typedef struct {
    Camera camera;
    Skybox skybox;
    SunLight sun;

    Player player;
    GameEntity entities[225];
    int num_entities;
} Game;

Game game_init(void);
void game_distroy(Game *game);
void game_load_level(Game *game);

#endif
