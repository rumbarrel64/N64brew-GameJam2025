// General file to hold all libraries
#include "libs.h"
#include "menu.h"
#include "gameState.h"


void menu_loop() {   

        static int selected_option = 0;
        
        // Lock display and clear screen
        surface_t *disp = display_get();

        /* Drawing */
        graphics_fill_screen(disp, graphics_make_color(0, 0, 50, 255));
        graphics_set_color(graphics_make_color(255, 255, 0, 255), 0);

        graphics_draw_text(disp, 80, 40, "FALLOUT VAULT 64");
        graphics_draw_text(disp, 100, 100, (selected_option == 0) ? "> START" : "  START");
        graphics_draw_text(disp, 100, 120, (selected_option == 1) ? "> TUTORIAL" : "  TUTORIAL");

        // Display
        display_show(disp);

        // Input Handling
        joypad_poll();
        joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        // Navagation
        if (btn.d_up)   selected_option = 0;
        if (btn.d_down) selected_option = 1;
        
        // Selection Logic (Checks for A OR Start)
        if (btn.a || btn.start) {
            if (selected_option == 0) {
                state = STATE_GAME;      // Jump straight to game
                return;
            } 
            else if (selected_option == 1) {
                state = STATE_TUTORIAL;  // Jump to your new tutorial screen
                return;
            }
        }
};