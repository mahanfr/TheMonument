#include "player.h"
#include "engine_cmd_line.h"
#include "rmanager.h"
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include "sun_light.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include <string.h>

static const float mouseSensitivity = 0.033f;
static const float rollSpeed = 2.5f;
static const float thrust = 35.0f;
static const float damping = 0.995f;
static const float steeringResponsiveness = 20.0f;

static const float cameraDistance = 22.0f;
static const float cameraHeight = 10.3f;

void player_init(Game *game) {
    Player *player = malloc(sizeof(Player));
    memset(player, 0, sizeof(Player));
    player->position = (Vector3) {0.0f, 0.0f, 0.0f};
    player->rotation = (Quaternion) { 0.0f, 0.0f, 0.0f, 1.0f };
    player->model = LoadModel(ModelPaths[RES_MODEL_SPACESHIPV1]);
    player->velocity = (Vector3) { 0.0f, 0.0f, 0.0f };
    player->angularVelocity = (Vector3) { 0.0f, 0.0f, 0.0f };

    // thrusters light shaders
    Shader tlshader = LoadShader(
            "./assets/shaders/glsl330/lighting.vs",
            "./assets/shaders/glsl330/lighting.fs");

    tlshader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(tlshader, "viewPos");
    int ambientLoc = GetShaderLocation(tlshader, "ambient");
    SetShaderValue(tlshader, ambientLoc, (float[4]){ 0.0f, 0.0f, 0.0f, 0.0f }, SHADER_UNIFORM_VEC4);
    nob_da_append(&player->localShaders, tlshader);
    Light light1 = CreateLight(LIGHT_POINT, Vector3Zero(), Vector3Zero(), GREEN, nob_da_last(&player->localShaders));
    nob_da_append(&player->lights, light1);
    Light light2 = CreateLight(LIGHT_POINT, Vector3Zero(), Vector3Zero(), GREEN, nob_da_last(&player->localShaders));
    nob_da_append(&player->lights, light2);

    game->player = player;
}

void player_handle_controls(Game *game) {
    Player *player = game->player;

    float dt = GetFrameTime();
    Vector2 mouse = GetMouseDelta();
    float targetYawRate   = -mouse.x * mouseSensitivity;
    float targetPitchRate = -mouse.y * mouseSensitivity;
    if (fabsf(mouse.x) < 1.0f) targetYawRate = 0;
    if (fabsf(mouse.y) < 1.0f) targetPitchRate = 0;

    player->angularVelocity.y += (targetYawRate   - player->angularVelocity.y) * steeringResponsiveness * dt;
    player->angularVelocity.x += (targetPitchRate - player->angularVelocity.x) * steeringResponsiveness * dt;
    player->angularVelocity.x = Clamp(player->angularVelocity.x, -2.5f, 2.5f);
    player->angularVelocity.y = Clamp(player->angularVelocity.y, -2.5f, 2.5f);

    // --- local axes from current orientation ---
    Vector3 forward = Vector3RotateByQuaternion((Vector3){1, 0, 0}, player->rotation);
    Vector3 up      = Vector3RotateByQuaternion((Vector3){0, 0, 1}, player->rotation);
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(up, forward));

    // --- mouse look (LOCAL axes) ---
    Quaternion qYaw   = QuaternionFromAxisAngle(up,    player->angularVelocity.y * dt);
    Quaternion qPitch = QuaternionFromAxisAngle(right, player->angularVelocity.x * dt);

    player->rotation = QuaternionMultiply(qYaw,   player->rotation);
    player->rotation = QuaternionMultiply(qPitch, player->rotation);

    // --- roll (LOCAL forward axis) ---
    if (IsKeyDown(KEY_A)) {
        Quaternion qRoll = QuaternionFromAxisAngle(forward, rollSpeed * dt);
        player->rotation = QuaternionMultiply(qRoll, player->rotation);
    }
    if (IsKeyDown(KEY_D)) {
        Quaternion qRoll = QuaternionFromAxisAngle(forward, -rollSpeed * dt);
        player->rotation = QuaternionMultiply(qRoll, player->rotation);
    }

    player->rotation = QuaternionNormalize(player->rotation);

    // --- recompute basis after rotation ---
    forward = Vector3RotateByQuaternion((Vector3){1, 0, 0}, player->rotation);
    up      = Vector3RotateByQuaternion((Vector3){0, 0, 1}, player->rotation);
    right   = Vector3Normalize(Vector3CrossProduct(up, forward));

    // --- thrust ---
    if (IsKeyDown(KEY_W))
        player->velocity = Vector3Add(player->velocity, Vector3Scale(forward, thrust * dt));
    if (IsKeyDown(KEY_S))
        player->velocity = Vector3Subtract(player->velocity, Vector3Scale(forward, thrust * dt));

    player->position = Vector3Add(player->position, Vector3Scale(player->velocity, dt));
    player->velocity = Vector3Scale(player->velocity, damping);

    // --- lights ---
    if (fabsf(player->velocity.x) > 10.0f) {
        player->lights.items[0].enabled = true;
        player->lights.items[1].enabled = true;
    } else {
        player->lights.items[0].enabled = false;
        player->lights.items[1].enabled = false;
    }

    {
        Vector3 lightOffsetLocal = { -6.0f, 5.0f, 0.0f };
        Vector3 lightOffsetWorld =
            Vector3RotateByQuaternion(lightOffsetLocal, player->rotation);
        player->lights.items[0].position =
            Vector3Add(player->position, lightOffsetWorld);
        UpdateLightValues(player->localShaders.items[0], player->lights.items[0]);
    }
    {
        Vector3 lightOffsetLocal = { -6.0f, -5.0f, 0.0f };
        Vector3 lightOffsetWorld =
            Vector3RotateByQuaternion(lightOffsetLocal, player->rotation);
        player->lights.items[1].position =
            Vector3Add(player->position, lightOffsetWorld);
        UpdateLightValues(player->localShaders.items[0], player->lights.items[1]);
    }

    // --- camera ---
    Vector3 cameraPos =
    Vector3Subtract(player->position, Vector3Scale(forward, cameraDistance));

    // lift camera upward in ship-local space
    cameraPos = Vector3Add(cameraPos, Vector3Scale(up, 8.0f));

    game->camera.position = cameraPos;
    game->camera.target   = Vector3Add(player->position, Vector3Scale(forward, 10.0f));
    game->camera.up       = up;
}

static void draw_player_model(Game *game) {
    Quaternion modelCorrectionFlip =
    QuaternionFromAxisAngle((Vector3){0, 0, 1}, PI);

    Quaternion modelCorrection =
    QuaternionMultiply(
        modelCorrectionFlip,
        QuaternionMultiply(
            QuaternionFromAxisAngle((Vector3){1, 0, 0}, PI / 2.0f),   // wings flat
            QuaternionFromAxisAngle((Vector3){0, 1, 0}, -PI / 2.0f)  // nose forward
        )
    );

    Matrix shipTransform =
            MatrixMultiply(
                QuaternionToMatrix(QuaternionMultiply(game->player->rotation, modelCorrection)),
                MatrixTranslate(game->player->position.x, game->player->position.y, game->player->position.z)
            );
    rlPushMatrix();
        rlMultMatrixf(MatrixToFloat(shipTransform));
        DrawModelEx(game->player->model, Vector3Zero(), Vector3Zero(), 0, Vector3One(), WHITE);
    rlPopMatrix();
}

void player_draw(Game *game) {
    sunlight_reapply(game, game->player->model);
    draw_player_model(game);
    BeginBlendMode(BLEND_ADDITIVE);
        game->player->model.materials[0].shader = game->player->localShaders.items[0];
        draw_player_model(game);
    EndBlendMode();
    if (game->is_edit_mode) {
        Lights lights = game->player->lights;
        for(size_t i = 0; i < lights.count; ++i) {
            DrawSphereEx(lights.items[i].position, 0.2f, 8, 8, lights.items[i].color);
        }
    }
}

void player_distroy(Player *player) {
    UnloadModel(player->model);
    free(player);
}

// Vector2 mouseDelta = GetMouseDelta();
// mouseDelta = Vector2Normalize(mouseDelta);
// if (mouseDelta.y != 0) {
//     yaw_momentom += 0.01 * -mouseDelta.y;
// } else {
//     yaw_momentom += -0.1 * yaw_momentom;
// }
// if (mouseDelta.x != 0) {
//     pitch_momentom += 0.01 * -mouseDelta.x;
// } else {
//     pitch_momentom += -0.1 * pitch_momentom;
// }
