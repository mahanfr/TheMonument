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
    player_init(game);
    level_load(game);
    sunlight_apply(game);
}

void game_render(Game *game) {
    DrawModel(game->player->model, game->player->position, 1.0f, WHITE);
    for (size_t i = 0; i < game->entities.count; ++i) {
        GameEntity entity = game->entities.items[i];
        DrawModel(entity.model, entity.position, 1.0f, WHITE);
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
