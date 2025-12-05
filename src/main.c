#include "../raylib/include/raylib.h"

typedef enum {
    SPRITE_DIR_UP = 0,
    SPRITE_DIR_DOWN,
    SPRITE_DIR_RIGHT,
    SPRITE_DIR_LEFT,
} SpriteDir;

typedef struct {
    Transform transform;
    Texture2D tIdle[4];
    Rectangle spriteFrame;
} Player;

void loadPlayerIdleTexture(Player *player) {
    const char* locUp    = "./assets/player/idle/idle_up.png";
    const char* locDown  = "./assets/player/idle/idle_down.png";
    const char* locRight = "./assets/player/idle/idle_right.png";
    const char* locLeft  = "./assets/player/idle/idle_left.png";
    player->tIdle[SPRITE_DIR_UP]    = LoadTexture(locUp);
    player->tIdle[SPRITE_DIR_DOWN]  = LoadTexture(locDown);
    player->tIdle[SPRITE_DIR_RIGHT] = LoadTexture(locRight);
    player->tIdle[SPRITE_DIR_LEFT]  = LoadTexture(locLeft);
}

void unloadPlayerIdleTexture(Player *player) {
    UnloadTexture(player->tIdle[SPRITE_DIR_UP]);
    UnloadTexture(player->tIdle[SPRITE_DIR_DOWN]);
    UnloadTexture(player->tIdle[SPRITE_DIR_RIGHT]);
    UnloadTexture(player->tIdle[SPRITE_DIR_LEFT]);
}

int main(void) {
    InitWindow(800, 600, "The Monument");

    Player player = {0};
    player.transform.translation.x = GetScreenWidth() / 2.0f - 16;
    player.transform.translation.y = GetScreenHeight() / 2.0f - 16;
    loadPlayerIdleTexture(&player);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime() * 60;
        if (IsKeyDown(KEY_UP)) {
            player.transform.translation.y -= 2 * deltaTime;
        } else if (IsKeyDown(KEY_DOWN)) {
            player.transform.translation.y += 2 * deltaTime;
        } else if (IsKeyDown(KEY_RIGHT)) {
            player.transform.translation.x += 2 * deltaTime;
        } else if (IsKeyDown(KEY_LEFT)) {
            player.transform.translation.x -= 2 * deltaTime;
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(player.transform.translation.x, player.transform.translation.y, 32, 32, RED);
        EndDrawing();
    }
    unloadPlayerIdleTexture(&player);
    CloseWindow();
    return 0;
}
