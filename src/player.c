#include "player.h"
#include "rmanager.h"
#include <stdlib.h>
#include <raylib.h>

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
    Vector3 offset = (Vector3) {0.0f, 6.0f, -20.0f};
    game->camera.position = Vector3Add(playerPos, offset);
}

static Vector3 velocity = (Vector3) {0,0,0};
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
    game->player->position = Vector3Add(game->player->position, velocity);
}

void player_distroy(Player *player) {
    UnloadModel(player->model);
    free(player);
}
