#include "libs.h"
#include "physics.h"

// For now physics will be check in the players and enemies

// Collision functions
void vault_check(T3DVec3 *pos, float size, float comp) {
    // Main Walls Check
    if(pos->v[0] < -size) pos->v[0] = -size;
    if(pos->v[0] >  size) pos->v[0] =  size;
    if(pos->v[2] < -size) pos->v[2] = -size;
    if(pos->v[2] >  size) pos->v[2] =  size;

    // Computer Check
    if(pos->v[0] > comp && pos->v[2] < -comp) {
        
        // Front wall of computer (Z axis)
        if(pos->v[2] > -(comp + 5.0f)) { 
            pos->v[2] = -comp; 
        } 
        // Side wall of computer (X axis)
        else if(pos->v[0] < (comp + 5.0f)) {
            pos->v[0] = comp;
        }
    }
}

bool enemy_check(T3DVec3 *pos, T3DVec3 objPos, float radius) {
    float dx = pos->v[0] - objPos.v[0];
    float dz = pos->v[2] - objPos.v[2];
    float distSq = (dx * dx) + (dz * dz);

    if(distSq < (radius * radius) && distSq > 0.01f) {
        float dist = sqrtf(distSq);
        float overlap = radius - dist;

        // Push player out
        pos->v[0] += (dx / dist) * overlap;
        pos->v[2] += (dz / dist) * overlap;
        return true;
    }
    return false;
}

bool projectile_check(T3DVec3 bulletPos, T3DVec3 playerPos, float radius) {
    float dx = bulletPos.v[0] - playerPos.v[0];
    float dz = bulletPos.v[2] - playerPos.v[2];
    float distSq = (dx * dx) + (dz * dz);

    return (distSq < (radius * radius));
}