#include "../raylib/include/raylib.h"
#include "../raylib/include/rlgl.h"

int main(void) {
    InitWindow(800, 600, "The Monument");

    Model model = LoadModel("./assets/SpaceShipV1.obj");
    Camera3D camera = {0};
    camera.position = (Vector3){ 5.0f, 4.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

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

    Image sb_image = LoadImage("./assets/skybox.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(sb_image, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(sb_image);

    // BASIC LIGHTING
    Shader shdrLight = LoadShader(
            "./assets/shaders/glsl330/basic_lighting.vs",
            "./assets/shaders/glsl330/basic_lighting.fs");
    float ambientStrength = 0.5;
    Vector4 ambientColor = ColorNormalize(RAYWHITE);
    SetShaderValue(
            shdrLight,
            GetShaderLocation(shdrLight, "ambientColor"),
            &(Vector3) {ambientColor.x, ambientColor.y, ambientColor.z},
            SHADER_UNIFORM_VEC3);
    SetShaderValue(shdrLight, GetShaderLocation(shdrLight, "ambientStrength"), &ambientStrength, SHADER_UNIFORM_FLOAT);
    model.materials[0].shader = shdrLight;

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
    UnloadShader(shdrLight);

    UnloadModel(skybox);
    UnloadModel(model);
    CloseWindow();
    return 0;
}
