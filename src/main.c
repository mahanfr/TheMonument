#include "game.h"
#include "player.h"
#include "sun_light.h"
#include "skybox.h"
#include <raylib.h>
#include <stddef.h>
#include "engine_cmd_line.h"
#define NOB_IMPLEMENTATION
#include "nob.h"

bool EDIT_CMD_MODE = false;

int main(void) {
    InitWindow(800, 600, "The Monument");

    Game *game = game_init();
    game_load_level(game);
    editor_cmd_init();

    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);
    DisableCursor();
    while (!WindowShouldClose()) {
        if (!EDIT_CMD_MODE && game->is_edit_mode)
            UpdateCamera(&game->camera, CAMERA_FIRST_PERSON);
        else player_update_camera(game);

        if (IsKeyPressed(KEY_F1)) game->is_edit_mode = !game->is_edit_mode;
        if (IsKeyPressed(KEY_GRAVE)) EDIT_CMD_MODE = !EDIT_CMD_MODE;
        if (EDIT_CMD_MODE) editor_cmd_handle_controls(game);
        else player_handle_controls(game);

        sunlight_update(game);
        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            BeginMode3D(game->camera);
                skybox_render(game->skybox);

                game_render(game);
                if (game->is_edit_mode) DrawGrid(20, 1.0f);
            EndMode3D();

            DrawFPS(10, 10);
            DrawText(
                    TextFormat("x: %.2f, y: %.2f, z: %.2f",
                        game->player->position.x,
                        game->player->position.y,
                        game->player->position.z),
                    10, 30, 10, WHITE);
            DrawText(
                    TextFormat("x: %.2f, y: %.2f, z: %.2f",
                        game->player->rotation.x,
                        game->player->rotation.y,
                        game->player->rotation.z),
                    10, 50, 10, WHITE);
            if (EDIT_CMD_MODE) editor_cmd_draw();
        EndDrawing();
    }
    game_distroy(game);
    editor_cmd_distroy();
    CloseWindow();
    return 0;
}
