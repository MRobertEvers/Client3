#pragma once

#include "entity.h"
#include "model.h"

typedef struct {
    bool temporary;
    int level;
    int y;
    int x;
    int z;
    Model *model;
    Entity *entity;
    int yaw;
    int minSceneTileX;
    int maxSceneTileX;
    int minSceneTileZ;
    int maxSceneTileZ;
    int distance;
    int cycle;
    int bitset;
    int8_t info;
} Location;


void location_free(Location *loc);