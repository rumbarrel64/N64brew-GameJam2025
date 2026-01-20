#include "libs.h"
#include "physics.h"

// For now physics will be check in the players and enemies

// Limits a position to a square area centered at 0,0
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