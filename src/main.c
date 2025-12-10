#include "../raylib/include/raylib.h"
#include "../raylib/include/rlgl.h"

int main(void) {
    InitWindow(800, 600, "The Monument");

    Model model = LoadModel("./assets/SpaceShipV1.obj");
    Camera3D camera = {0};
    camera.position = (Vector3){ 5.0f, 4.0f, 5.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model skybox = LoadModelFromMesh(cube);

    skybox.materials[0].shader = LoadShader(
            "./assets/shaders/glsl330/skybox.vs",
            "./assets/shaders/glsl330/skybox.fs");
    SetShaderValue(
            skybox.materials[0].shader,
            GetShaderLocation(skybox.materials[0].shader, "environmentMap"),
            (int[1]) {MATERIAL_MAP_CUBEMAP},
            SHADER_UNIFORM_INT);
    SetShaderValue(
            skybox.materials[0].shader,
            GetShaderLocation(skybox.materials[0].shader, "doGamma"),
            (int[1]) { 0 },
            SHADER_UNIFORM_INT);
    SetShaderValue(
            skybox.materials[0].shader,
            GetShaderLocation(skybox.materials[0].shader, "vflipped"),
            (int[1]) { 0 },
            SHADER_UNIFORM_INT);

    Shader shdrCubemap = LoadShader(
            "./assets/shaders/glsl330/cubemap.vs",
            "./assets/shaders/glsl330/cubemap.fs");
    SetShaderValue(
            shdrCubemap,
            GetShaderLocation(shdrCubemap, "equirectangularMap"),
            (int[1]){ 0 },
            SHADER_UNIFORM_INT);

    Image sb_image = LoadImage("./assets/skybox/corona_up.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(sb_image, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(sb_image);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));
            BeginMode3D(camera);
                rlDisableBackfaceCulling();
                rlDisableDepthMask();
                    DrawModel(skybox, (Vector3) {0,0,0}, 1.0f, WHITE);
                rlEnableBackfaceCulling();
                rlEnableDepthMask();

                DrawModel(model, (Vector3) {0}, 1.0f, WHITE);
                DrawGrid(10, 1.0f);        // Draw a grid
            EndMode3D();
        EndDrawing();
    }
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

    UnloadModel(skybox);
    UnloadModel(model);
    CloseWindow();
    return 0;
}
