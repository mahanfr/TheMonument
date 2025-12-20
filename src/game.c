#include "engine_cmd_line.h"
#include "player.h"
#include "sun_light.h"
#include "skybox.h"
#include "game.h"
#include "level.h"
#include <raylib.h>
#include <stdlib.h>

Game *game_init(void) {
    Game *game = malloc(sizeof(Game));
    Camera3D camera = {0};
    camera.position = (Vector3){ 5.0f, 4.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    game->camera = camera;

    game->skybox = skybox_init("./assets/skybox.png");
    game->sun = sunlight_init(GetColor(0x404040FF), WHITE, 32.0f);
    game->settings = (GameSettings) {.resolution = (Vector2) {GetScreenWidth(), GetScreenHeight()}};
    sunlight_set_direction(game->sun, (Vector3) {-0.3f,-1.0f,-0.3f});
    return game;
}

void game_load_level(Game *game) {
    // Model model = LoadModel(ModelPaths[RES_MODEL_SPACESHIPV1]);
    // GameEntity entity = (GameEntity) {
    //     .id = 0,
    //     .position = (Vector3) {10.0f, 0.0f, 0.0f},
    //     .model = model,
    //     .modelid = RES_MODEL_SPACESHIPV1,
    // };
    // da_append(&game->entities, entity);
    // level_save(game);
    Mesh planet_mesh = GenMeshSphere(100.0f, 20, 20);
    Model planet = LoadModelFromMesh(planet_mesh);
    Image image = LoadImage("./assets/textures/Gaseous_01.png");
    Texture texture = LoadTextureFromImage(image);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    SetMaterialTexture(&planet.materials[0], MATERIAL_MAP_ALBEDO, texture);
    planet.materials->maps->color = WHITE;
    GameEntity planet_entity = (GameEntity) {
        .id = 0,
        .position = (Vector3) {400.0, 0.0, 0.0},
        .rotation = (Vector3) {1, 1, 1},
        .scale = (Vector3) {1.0f, 1.0f, 1.0f},
        .model = planet,
        .modelid = 100,
    };
    nob_da_append(&game->entities, planet_entity);
    player_init(game);
    //level_load(game);
    sunlight_apply(game);
}

void game_render(Game *game) {
    // DrawModel(game->player->model, game->player->position, 1.0f, WHITE);
    player_draw(game);
    for (size_t i = 0; i < game->entities.count; ++i) {
        GameEntity entity = game->entities.items[i];
        // DrawModel(entity.model, entity.position, 1.0f, WHITE);
        DrawModelEx(entity.model, entity.position, entity.rotation, 0.0f, entity.scale, WHITE);
        if (game->is_edit_mode) {
            BoundingBox bound = GetModelBoundingBox(entity.model);
            DrawBoundingBox(bound, WHITE);
            Vector3 centerPos = Vector3Scale(Vector3Add(bound.max, bound.min), 0.5);
            DrawSphere(centerPos, 0.5f, RED);
            DrawCylinderEx(centerPos, Vector3Add(centerPos, (Vector3) {1.5f,0,0}), 0.09, 0, 10, GREEN);
            DrawCylinderEx(centerPos, Vector3Add(centerPos, (Vector3) {0,1.5f,0}), 0.09, 0, 10, RED);
            DrawCylinderEx(centerPos, Vector3Add(centerPos, (Vector3) {0,0,1.5f}), 0.09, 0, 10, YELLOW);
        }
    }
}

void game_distroy(Game *game) {
    player_distroy(game->player);
    for (int i = 0; i < game->entities.count; ++i) {
        UnloadModel(game->entities.items[i].model);
    }
    sunlight_distroy(game->sun);
    skybox_distroy(game->skybox);
}
