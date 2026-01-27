// spore.h
#pragma once

#include "libs.h"  // Provides access to T3D, libdragon, etc.

struct Player;

// The Projectile
typedef struct {
    T3DModel *model;
    T3DMat4FP *matrix;
    T3DVec3 position;
    T3DVec3 direction;
    float rotY;
    bool active;
} SporeBullet;

// The Shooter
typedef struct SporePlant {
    T3DModel *model;
    T3DMat4FP *matrix;
    T3DSkeleton skel;
    T3DAnim animIdle;
    T3DVec3 position;
    float rotY;
    float shootTimer;
    int health;
    bool active;
    SporeBullet bullet; // Each plant owns one bullet
} SporePlant;

void spores_init(SporePlant *plants, int count);
void spores_update(SporePlant *plants, int count, float deltaTime, struct Player *player);
void spores_draw(SporePlant *plants, int count);
void spores_cleanup(SporePlant *plants, int count);
