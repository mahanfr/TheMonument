#ifndef _REGISTERY_HEADER
#define _REGISTERY_HEADER
#pragma once

typedef enum ModelID {
#define X(id, path) id,
#include "_resources.h"
#undef X
    MODEL_COUNT
} ModelID;

extern const char *ModelPaths[MODEL_COUNT];
#endif
