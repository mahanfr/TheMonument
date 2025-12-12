#include "game.h"

Game game_init(void) {
    Game game = {0};
    Camera3D camera = {0};
    camera.position = (Vector3){ 5.0f, 4.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    game.camera = camera;

    game.skybox = skybox_init("./assets/skybox.png");
    game.sun = sunlight_init(GetColor(0x404040FF), WHITE, 32.0f);
    sunlight_set_direction(game.sun, (Vector3) {-0.3f,-1.0f,-0.3f});
    return game;
}

void game_load_level(Game *game) {
    Model model = LoadModel("./assets/SpaceShipV1.obj");
    GameEntity entity = (GameEntity) {
        .id = 0,
        .position = (Vector3) {10.0f, 0.0f, 0.0f},
        .model = model,
    };
    game->entities[game->num_entities] = entity;
    game->num_entities++;
    sunlight_apply(game->sun, (Model[]) {entity.model}, 1);
}

void game_distroy(Game *game) {
    for (int i = 0; i < game->num_entities; ++i) {
        UnloadModel(game->entities[i].model);
    }
    sunlight_distroy(game->sun);
    skybox_distroy(game->skybox);
}
