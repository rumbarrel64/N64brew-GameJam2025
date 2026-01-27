// player.h
#pragma once

#include "libs.h"  // Provides access to T3D, libdragon, etc.

struct SporePlant;

typedef struct Player {
    T3DModel *model;
    T3DSkeleton skel;
    T3DSkeleton skelBlend;
    T3DAnim animIdle;
    T3DAnim animWalk;
    T3DAnim animAttack;
    T3DMat4FP *matrix;    
    T3DVec3 position;
    T3DVec3 moveDir;
    float rotY;
    float currSpeed;
    float animBlend;
    bool isAttack;
    int health;
} Player;

void player_init(Player *p);
void player_update(Player *p, float deltaTime, joypad_inputs_t joy, joypad_buttons_t btn, struct SporePlant *spores, int sporeCount);
void player_draw(Player *p);
void player_cleanup(Player *p);