#include "game.h"

#define DEBUG_MODE true
bool EDIT_MODE = true;

int main(void) {
    InitWindow(800, 600, "The Monument");

    Game game = game_init();
    game_load_level(&game);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateCamera(&game.camera, CAMERA_FIRST_PERSON);

        sunlight_update(game.sun, game.camera.position);

        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            BeginMode3D(game.camera);
                skybox_render(game.skybox);

                for (int i = 0; i < game.num_entities; ++i) {
                    GameEntity entity = game.entities[i];
                    DrawModel(entity.model, entity.position, 1.0f, WHITE);
                }

                DrawGrid(10, 1.0f);
            EndMode3D();
        EndDrawing();
    }
    game_distroy(&game);
    CloseWindow();
    return 0;
}
