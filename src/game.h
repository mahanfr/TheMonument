#ifndef GAME_HEADER
#define GAME_HEADER

#include "raylib.h"
#include "stddef.h"

typedef Model Skybox;
typedef struct SunLight SunLight;

typedef struct {
    long id;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    int modelid;
    Model model;
} Player;

typedef struct {} Light;

typedef struct {
    long id;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    int modelid;
    Model model;
} GameEntity;

typedef struct{
    GameEntity *items;
    size_t count;
    size_t capacity;
} GameEntities;

typedef struct {
    Vector2 resolution;
} GameSettings;

typedef struct {
    Camera camera;
    Skybox *skybox;
    SunLight *sun;

    Player *player;
    GameEntities entities;
    bool is_edit_mode;
    GameSettings settings;
} Game;

Game *game_init(void);
void game_distroy(Game *game);
void game_load_level(Game *game);
void game_render(Game *game);

#endif
