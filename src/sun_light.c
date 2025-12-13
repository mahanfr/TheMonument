#include "game.h"
#include "sun_light.h"
#include <stdlib.h>

struct SunLight {
    Shader shader;
};

SunLight *sunlight_init
(
    Color ambient,
    Color light,
    float shininess
) {
    SunLight *sun = malloc(sizeof(SunLight));
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

    sun->shader = shader;
    return sun;
}

void sunlight_apply(Game *game) {
    for(int i=0; i < game->entities.count; i++) {
        game->entities.items[i].model.materials[0].shader = game->sun->shader;
    }
}

void sunlight_set_direction(SunLight *sun, Vector3 new_direction) {
    int lightDirectionLoc = GetShaderLocation(sun->shader,"lightDirection");
    SetShaderValue(sun->shader, lightDirectionLoc, &new_direction, SHADER_UNIFORM_VEC3);
}

void sunlight_update(SunLight *sun, Vector3 camera_position) {
    int viewPositionLoc   = GetShaderLocation(sun->shader,"viewPosition");
    SetShaderValue(sun->shader, viewPositionLoc, &camera_position, SHADER_UNIFORM_VEC3);
}

void sunlight_distroy(SunLight *sun) {
    UnloadShader(sun->shader);
    free(sun);
}
