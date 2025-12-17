#include "player.h"
#include "rmanager.h"
#include <stdlib.h>
#include <raylib.h>

static Vector3 velocity = (Vector3) {0,0,0};
static float roll = 0.0f;
static Vector3 cameraOffset = (Vector3) {0.0f, 6.0f, -20.0f};

void player_init(Game *game) {
    Player *player = malloc(sizeof(Player));
    player->position = Vector3Zero();
    player->rotation = Vector3Zero();
    player->model = LoadModel(ModelPaths[RES_MODEL_SPACESHIPV1]);
    game->player = player;
}

void player_update_camera(Game *game) {
    Vector3 playerPos = game->player->position;
    game->camera.target = playerPos;
    Vector3 rotatedOffset = {
            cameraOffset.x * cosf(roll) - cameraOffset.y * sinf(roll),
            cameraOffset.x * sinf(roll) + cameraOffset.y * cosf(roll),
            cameraOffset.z
    };
    game->camera.position = Vector3Add(playerPos, rotatedOffset);
    game->camera.up = (Vector3) {-sinf(roll), cosf(roll), 0.0f};
}

void player_handle_controls(Game *game) {
    float delta_time = GetFrameTime() * 60;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        if (velocity.z < 1) velocity.z += 0.01 * delta_time;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        if (velocity.z > 0)
            velocity.z -= 0.01 * delta_time;
        else
            velocity.z = 0;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        roll += 0.01 * delta_time;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        roll -= 0.01 * delta_time;
    }
    game->player->position = Vector3Add(game->player->position, velocity);
    game->player->rotation.z = roll;
}

void player_distroy(Player *player) {
    UnloadModel(player->model);
    free(player);
}
