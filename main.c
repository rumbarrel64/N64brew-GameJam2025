// General file to hold all libraries
#include "libs.h"
#include "menu.h"
#include "vault64.h"
#include "gameState.h"

// Set the game state to menu
GameState state = STATE_MENU;

int main(void) {
    /* 1. System Initializations */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    joypad_init();
    rdpq_init();
    dfs_init(DFS_DEFAULT_LOCATION); // Essential for the Makefile's filesystem

    while (1) {

        switch (state) {
            case STATE_MENU:
                menu_loop();
                break;
            case STATE_GAME:
                play_loop();
                break;
            // Add other states here
            default:
                break;
        }
        
    };

    return 0;
};

