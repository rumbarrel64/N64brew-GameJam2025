// physics.h
#pragma once

// Function to check collisions between players/and enemies and the vault
void vault_check(T3DVec3 *pos, // Player or Enemy Coordinates
                 float size,   // Room Size
                 float comp    // Computer Size
);


bool enemy_check(T3DVec3 *pos, 
                 T3DVec3 objPos, 
                 float radius
);