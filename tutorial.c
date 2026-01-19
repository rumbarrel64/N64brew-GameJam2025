// General file to hold all libraries
#include "libs.h"
#include "tutorial.h"
#include "gameState.h"
#include "physics.h"

void tutorial_loop() {
    joypad_poll();
    joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    // If they press A or Start, go to the game
    if (btn.start) {
        state = STATE_GAME;
    }
    else if (btn.b) {
        state = STATE_MENU;  // Jump to your new tutorial screen
    };

    rdpq_attach(display_get(), display_get_zbuf());
    t3d_frame_start();
    
    rdpq_set_mode_fill(RGBA32(0, 0, 0, 255)); // Black background
    rdpq_fill_rectangle(0, 0, 320, 240);
    
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 40, "MISSION BRIEFING: VAULT 64");
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 70, "The Vault is Overrun and needs repair! Purge the critters.");
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 85, "Collect the parts. Repair the Computer.");
    
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 130, "CONTROLS:");
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 145, "A: ATTACK");
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 160, "B: REPAIR COMPUTER");
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 175, "JOYSTICK: MOVE");
    
    rdpq_text_print(NULL, FONT_BUILTIN_DEBUG_MONO, 30, 210, "Press START to Begin or B to Go back to Menu");

    rdpq_detach_show();
}