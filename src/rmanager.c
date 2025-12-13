#include "rmanager.h"

const char *ModelPaths[MODEL_COUNT] = {
#define X(id, path) [id] = path,
#include "_resources.h"
#undef X
};

