#include "skybox.h"

Skybox skybox_init(const char* cubemap) {
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

    Image sb_image = LoadImage(cubemap);
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(sb_image, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(sb_image);
    return skybox;
}

void skybox_render(Skybox skybox) {
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skybox, (Vector3) {0,0,0}, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

void skybox_distroy(Skybox skybox) {
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);
}

