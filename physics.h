// physics.h
#pragma once

// Function to keep a position within the outer vault walls
void vault_check(T3DVec3 *pos, // Player Coordinates
                 float size,   // Room Size
                 float comp    // Computer Size
);   