#include "player.h"
#include "rmanager.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

Vector3 position = { 0 };
Vector3 velocity = { 0 };

Quaternion orientation;

const float mouseSensitivity = 0.003f;
const float rollSpeed = 2.5f;
const float thrust = 25.0f;
const float damping = 0.995f;

const float cameraDistance = 22.0f;

void player_init(Game *game) {
    Player *player = malloc(sizeof(Player));
    player->position = Vector3Zero();
    player->rotation = Vector3Zero();
    player->model = LoadModel(ModelPaths[RES_MODEL_SPACESHIPV1]);
    game->player = player;
    orientation = QuaternionIdentity();
}

void player_update_camera(Game *game) {
}

void player_handle_controls(Game *game) {
    float dt = GetFrameTime();
    Vector2 mouse = GetMouseDelta();

    // --- local axes from current orientation ---
    Vector3 forward = Vector3RotateByQuaternion((Vector3){1, 0, 0}, orientation);
    Vector3 up      = Vector3RotateByQuaternion((Vector3){0, 0, 1}, orientation);
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(up, forward));

    // --- mouse look (LOCAL axes) ---
    Quaternion qYaw   = QuaternionFromAxisAngle(up,    -mouse.x * mouseSensitivity);
    Quaternion qPitch = QuaternionFromAxisAngle(right, -mouse.y * mouseSensitivity);

    orientation = QuaternionMultiply(qYaw, orientation);
    orientation = QuaternionMultiply(qPitch, orientation);

    // --- roll (LOCAL forward axis) ---
    if (IsKeyDown(KEY_A)) {
        Quaternion qRoll = QuaternionFromAxisAngle(forward, rollSpeed * dt);
        orientation = QuaternionMultiply(qRoll, orientation);
    }
    if (IsKeyDown(KEY_D)) {
        Quaternion qRoll = QuaternionFromAxisAngle(forward, -rollSpeed * dt);
        orientation = QuaternionMultiply(qRoll, orientation);
    }

    orientation = QuaternionNormalize(orientation);

    // --- recompute basis after rotation ---
    forward = Vector3RotateByQuaternion((Vector3){1, 0, 0}, orientation);
    up      = Vector3RotateByQuaternion((Vector3){0, 0, 1}, orientation);

    // --- thrust ---
    if (IsKeyDown(KEY_W))
        velocity = Vector3Add(velocity, Vector3Scale(forward, thrust * dt));
    if (IsKeyDown(KEY_S))
        velocity = Vector3Subtract(velocity, Vector3Scale(forward, thrust * dt));

    position = Vector3Add(position, Vector3Scale(velocity, dt));
    velocity = Vector3Scale(velocity, damping);

    // --- camera ---
    game->camera.position = Vector3Subtract(position, Vector3Scale(forward, cameraDistance));
    game->camera.target   = Vector3Add(position, forward);
    game->camera.up       = up;
}

void player_draw(Game *game) {
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
                QuaternionToMatrix(QuaternionMultiply(orientation, modelCorrection)),
                MatrixTranslate(position.x, position.y, position.z)
            );
    rlPushMatrix();
        rlMultMatrixf(MatrixToFloat(shipTransform));
        DrawModelEx(game->player->model, Vector3Zero(), Vector3Zero(), 0, Vector3One(), WHITE);
    rlPopMatrix();
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
