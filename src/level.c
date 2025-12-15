#include <stdio.h>
#include "level.h"
#include "array.h"
#include "rmanager.h"
#include "game.h"
#include <errno.h>
#include <string.h>
#include <stddef.h>

typedef struct {
    unsigned char* items;
    long count;
    long capacity;
} Bytes;

typedef enum {
    LEVEL_TOKEN_META = 0,
    LEVEL_TOKEN_ENTITY,
} LevelToken;

// LevelToken 8 >> id 32 >> position 32 32 32 >> model_id 32 >> end
static void write_entity(FILE *f, GameEntity *entity) {
    LevelToken token = LEVEL_TOKEN_ENTITY;
    fwrite(&token, sizeof(LevelToken), 1, f);
    fwrite(&entity->id, sizeof(int), 1, f);
    fwrite(&entity->position, sizeof(Vector3), 1, f);
    fwrite(&entity->modelid, sizeof(int), 1, f);
}

static GameEntity load_entity(FILE *f) {
    int id;
    fread(&id, sizeof(id), 1, f);
    Vector3 position;
    fread(&position, sizeof(position), 1, f);
    int model_id;
    fread(&model_id, sizeof(model_id), 1, f);
    Model model = LoadModel(ModelPaths[model_id]);
    GameEntity entity = (GameEntity) {
        .id = id,
        .position = position,
        .model = model,
        .modelid = model_id,
    };
    return entity;
}

void level_save(Game *game) {
    FILE *file = fopen("level.dat", "wb");
    if (!file) {
        TraceLog(LOG_ERROR,
                "Failed to open file 'level.dat': %s",
                strerror(errno));
        return;
    }
    for (int i = 0; i < game->entities.count; ++i) {
        write_entity(file, &game->entities.items[i]);
    }
    fclose(file);
}

void level_load(Game *game) {
    FILE *file = fopen("level.dat", "rb");
    if (!file) {
        TraceLog(LOG_ERROR,
                "Failed to open file 'level.dat': %s",
                strerror(errno));
        return;
    }

    LevelToken token;
    while (true) {
        size_t read = fread(&token, sizeof(token), 1, file);
        if (read == 0) {
            break;
        }
        switch (token) {
            case LEVEL_TOKEN_META:
                break;
            case LEVEL_TOKEN_ENTITY: {
                    GameEntity entity = load_entity(file);
                    da_append(&game->entities, entity);
                }
                break;
            default:
            TraceLog(LOG_ERROR,
                    "Unknown Level structure (%d) %s:%d", token, __FILE__, __LINE__);
                break;
        }
    }

}
