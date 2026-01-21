// sporePlant.h
#pragma once

#include "libs.h"  // Provides access to T3D, libdragon, etc.

typedef struct {
    T3DModel *model;
    T3DMat4FP *matrix;
    T3DSkeleton skel;
    T3DAnim animIdle;
    T3DVec3 position;
    float rotY;
} SporePlant;

void spore_init(SporePlant *plants, int count);
void spore_update(SporePlant *plants, int count, float deltaTime, T3DVec3 playerPos);
void spore_draw(SporePlant *plants, int count);
void spore_cleanup(SporePlant *plants, int count);
