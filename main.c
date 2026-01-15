// General file to hold all libraries
#include "libs.h"
#include "menu.h"
#include "game.h"
#include "gameState.h"
#include "tutorial.h"

// Set the game state to menu
GameState state = STATE_MENU;

int main(void) {
    // System Initializations
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    joypad_init();
    rdpq_init();
    dfs_init(DFS_DEFAULT_LOCATION); // Essential for the Makefile's filesystem
    asset_init_compression(2);
    t3d_init((T3DInitParams){});

    // Load Font Data
    rdpq_text_register_font(FONT_BUILTIN_DEBUG_MONO, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));

    // Main game loop, determines if in menu, tutorial or in game
    while (1) {
        
        switch (state) {
            case STATE_MENU:
                menu_loop();
                break;
            case STATE_TUTORIAL:
                tutorial_loop();
                break;
            case STATE_GAME:
                play_loop();
                break;
        }
    }
    return 0;
}

