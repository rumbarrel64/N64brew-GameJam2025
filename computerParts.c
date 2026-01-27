#include "computerParts.h"

static T3DModel *partModel = NULL;

void computerParts_init(ComputerPart *parts, int count, SporePlant *plants) {
    partModel = t3d_model_load("rom:/computerParts.t3dm");
    for(int i = 0; i < count; i++) {
        parts[i].matrix = malloc_uncached(sizeof(T3DMat4FP));
        parts[i].collected = false;
        
        // Position the part where the plant is, but slightly lifted
        parts[i].position = plants[i].position;
        parts[i].position.v[1] = 2.0f; 

        // Build the matrix once since it doesn't move or rotate
        t3d_mat4fp_from_srt_euler(parts[i].matrix,
            (float[3]){0.2f, 0.2f, 0.2f}, // Scale
            (float[3]){0, 0, 0},          // No rotation
            parts[i].position.v           // Position
        );
    }
}

void computerParts_update(ComputerPart *parts, int count, Player *player, SporePlant *plants) {
    for(int i = 0; i < count; i++) {
        // Logic: Only "active" if plant is dead (active == false) and part isn't collected
        if(plants[i].active || parts[i].collected) continue;

        // Simple distance check for pickup
        float dx = player->position.v[0] - parts[i].position.v[0];
        float dz = player->position.v[2] - parts[i].position.v[2];
        float distSq = (dx * dx) + (dz * dz);

        // 900.0f = 30 units radius
        if(distSq < 900.0f) {
            parts[i].collected = true;
            player->repairPartsCount++; // Matches your updated player.h
        }
    }
}

// Added to fix "undefined reference" linker error
void computerParts_draw(ComputerPart *parts, int count, SporePlant *plants) {
    for(int i = 0; i < count; i++) {
        // Only draw if the plant is gone and the player hasn't touched the part
        if(!plants[i].active && !parts[i].collected) {
            t3d_matrix_push(parts[i].matrix);
            t3d_model_draw(partModel);
            t3d_matrix_pop(1);
        }
    }
}

// Added to fix "undefined reference" linker error
void computerParts_cleanup(ComputerPart *parts, int count) {
    for(int i = 0; i < count; i++) {
        if(parts[i].matrix) {
            free_uncached(parts[i].matrix);
            parts[i].matrix = NULL;
        }
    }
    if(partModel) {
        t3d_model_free(partModel);
        partModel = NULL;
    }
}