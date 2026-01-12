// General file to hold all libraries
#include "libs.h"
#include "menu.h"
#include "gameState.h"


void menu_loop() {   

        static int selected_option = 0;
        
        /* Lock display and clear screen */
        surface_t *disp = display_get();

        /* Drawing */
        graphics_fill_screen(disp, graphics_make_color(0, 0, 50, 255));
        graphics_set_color(graphics_make_color(255, 255, 0, 255), 0);

        graphics_draw_text(disp, 80, 40, "Fallout Vault 64");
        graphics_draw_text(disp, 100, 100, (selected_option == 0) ? "> START" : "  START");
        graphics_draw_text(disp, 100, 120, (selected_option == 1) ? "> EXIT" : "  EXIT");

        /* Display and Handle Input */
        display_show(disp);

        joypad_poll();
        
        // Get buttons for the first controller (JOYPAD_PORT_1)
        joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (btn.d_up)   selected_option = 0;
        if (btn.d_down) selected_option = 1;
        
        if (btn.a) {
            if(selected_option == 0) {
            // state = STATE_PLAY; // This will work once state is shared
        }
        }

    };