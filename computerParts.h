// spore.h
#pragma once

#include "libs.h"  // Provides access to T3D, libdragon, etc.
#include "player.h" 
#include "spores.h" 

typedef struct {
    T3DVec3 position;
    T3DMat4FP *matrix;
    bool collected;
} ComputerPart;

void computerParts_init(ComputerPart *parts, int count, SporePlant *plants);
void computerParts_update(ComputerPart *parts, int count, Player *player, SporePlant *plants);
void computerParts_draw(ComputerPart *parts, int count, SporePlant *plants);
void computerParts_cleanup(ComputerPart *parts, int count);