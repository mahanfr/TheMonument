#include "game.h"
#include "sun_light.h"
#include "skybox.h"
#include <raylib.h>
#include <stddef.h>
#include "engine_cmd_line.h"
#define NOB_IMPLEMENTATION
#include "nob.h"

bool EDIT_MODE = true;

int main(void) {
    InitWindow(800, 600, "The Monument");

    Game *game = game_init();
    game_load_level(game);
    editor_cmd_init();

    SetTargetFPS(60);
    Nob_String_Builder command = {0};

    while (!WindowShouldClose()) {
        if (!EDIT_MODE) {
            UpdateCamera(&game->camera, CAMERA_FIRST_PERSON);
        }

        sunlight_update(game->sun, game->camera.position);

        if (EDIT_MODE) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (command.count > 0) {
                    command.items[command.count-1] = 0;
                }
                if (command.count > 0) command.count--;
            } else if (IsKeyPressed(KEY_ENTER)) {
                editor_cmd_run(game, command.items);
                command.items[0] = 0;
                command.count = 0;
            } else {
                char ch = GetCharPressed();
                if (ch != 0) {
                    nob_sb_appendf(&command, "%c", ch);
                }
            }
        }
        if (IsKeyPressed(KEY_F1)) EDIT_MODE = !EDIT_MODE;


        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            BeginMode3D(game->camera);
                skybox_render(game->skybox);

                for (size_t i = 0; i < game->entities.count; ++i) {
                    GameEntity entity = game->entities.items[i];
                    DrawModel(entity.model, entity.position, 1.0f, WHITE);
                }

                DrawGrid(10, 1.0f);
            EndMode3D();

            DrawFPS(10, 10);
            if (EDIT_MODE) editor_cmd_draw(game, &command);
        EndDrawing();
    }
    game_distroy(game);
    editor_cmd_distroy();
    CloseWindow();
    return 0;
}
