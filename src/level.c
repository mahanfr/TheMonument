
// Model model = LoadModel("./assets/SpaceShipV1.obj");
// GameEntity entity = (GameEntity) {
//     .id = 0,
//     .position = (Vector3) {10.0f, 0.0f, 0.0f},
//     .model = model,
// };

#include "game.h"
#include "nob.h"
#include <stdio.h>

typedef struct {
    unsigned char* items;
    long count;
    long capacity;
} Bytes;

typedef enum {
    LEVEL_TOKEN_META = 0,
    LEVEL_TOKEN_ENTITY,
} LevelToken;

// LevelToken 8 >> id 32 >> position 32 32 32 >> model s=32 s*8 >> end
static void write_entity(FILE *f, GameEntity *entity) {
    fwrite((const void*) LEVEL_TOKEN_ENTITY, sizeof(LevelToken), 1, f);
    fwrite(&entity->id, sizeof(int), 1, f);
    fwrite(&entity->position, sizeof(Vector3), 1, f);
    fwrite(&e->strLen, sizeof(int), 1, f);
    fwrite(e->text, sizeof(char), e->strLen, f);
}

void save_level() {
    FILE *file = fopen("level.dat", "rb");
}
