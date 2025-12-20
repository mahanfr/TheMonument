#ifndef GAME_HEADER
#define GAME_HEADER

#include "raylib.h"
#include "stddef.h"
#include "../raylib/include/rlights.h"

typedef Model Skybox;
typedef struct SunLight SunLight;

typedef struct {
    Shader *items;
    size_t count;
    size_t capacity;
} Shaders;

typedef struct {
    Light *items;
    size_t count;
    size_t capacity;
} Lights;

typedef struct {
    long id;
    Vector3 position;
    Vector3 scale;
    int modelid;
    Model model;
    Vector3 velocity;
    Vector3 angularVelocity;
    Quaternion rotation;
    Shaders localShaders;
    Lights lights;
} Player;

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
void game_prerender(Game *game);

#endif
