#include "sun_light.h"

SunLight sunlight_init
(
    Color ambient,
    Color light,
    float shininess
) {
    SunLight sun = {0};
    Shader shader = LoadShader(
            "./assets/shaders/glsl330/dirlight3d_vs.glsl",
            "./assets/shaders/glsl330/dirlight3d_fs.glsl");

    int ambientLoc        = GetShaderLocation(shader,"ambientColor");
    int lightColorLoc     = GetShaderLocation(shader,"lightColor");
    int shininessLoc      = GetShaderLocation(shader,"shininess");

    Vector4 ambient_color_norm = ColorNormalize(ambient);
    Vector4 light_color_norm   = ColorNormalize(light);

    SetShaderValue(shader, ambientLoc, &ambient_color_norm, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, lightColorLoc, &light_color_norm, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, shininessLoc, &shininess, SHADER_UNIFORM_FLOAT);

    sun.shader = shader;
    return sun;
}

void sunlight_apply(SunLight sun, Model models[], int count) {
    for(int i = 0; i < count; ++i) {
        models[i].materials[0].shader = sun.shader;
    }
}

void sunlight_set_direction(SunLight sun, Vector3 new_direction) {
    int lightDirectionLoc = GetShaderLocation(sun.shader,"lightDirection");
    SetShaderValue(sun.shader, lightDirectionLoc, &new_direction, SHADER_UNIFORM_VEC3);
}

void sunlight_update(SunLight sun, Vector3 camera_position) {
    int viewPositionLoc   = GetShaderLocation(sun.shader,"viewPosition");
    SetShaderValue(sun.shader, viewPositionLoc, &camera_position, SHADER_UNIFORM_VEC3);
}

void sunlight_distroy(SunLight sun) {
    UnloadShader(sun.shader);
}
