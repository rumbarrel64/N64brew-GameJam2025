#include <stdio.h>
#include <libdragon.h>

int main(void) {
    /* 1. System Initializations */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    joypad_init();
    rdpq_init();
    dfs_init(DFS_DEFAULT_LOCATION); // Essential for the Makefile's filesystem

    int selected_option = 0;

    while (1) {
        /* 2. Lock display and clear screen */
        surface_t *disp = display_get();

        /* 3. Drawing */
        graphics_fill_screen(disp, graphics_make_color(0, 0, 50, 255));

        /* 3. Drawing */
        graphics_fill_screen(disp, graphics_make_color(0, 0, 50, 255));
        graphics_set_color(graphics_make_color(255, 255, 0, 255), 0);

        graphics_draw_text(disp, 80, 40, "Fallout Vault 64");
        graphics_draw_text(disp, 100, 100, (selected_option == 0) ? "> START" : "  START");
        graphics_draw_text(disp, 100, 120, (selected_option == 1) ? "> EXIT" : "  EXIT");

        /* 4. Display and Handle Input */
        display_show(disp);

        joypad_poll();
        
        // Get buttons for the first controller (JOYPAD_PORT_1)
        joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (keys.d_up)   selected_option = 0;
        if (keys.d_down) selected_option = 1;
        
        if (keys.a) {
            // Action for A button
        }
    }
}
