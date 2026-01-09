#include <stdio.h>
#include <libdragon.h>

int main(void) {
    /* 1. System Initializations */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    controller_init();
    dfs_init(DFS_DEFAULT_LOCATION); // Essential for the Makefile's filesystem

    int selected_option = 0;

    while (1) {
        /* 2. Lock display and clear screen */
        surface_t *disp = display_lock();
        if (!disp) continue;

        // Simple background fill using software graphics (easier for minimal text)
        graphics_fill_screen(disp, graphics_make_color(0, 0, 50, 255));

        /* 3. Draw Text */
        graphics_set_color(0xFFFFFFFF, 0);
        graphics_draw_text(disp, 80, 40, "N64 LIBDRAGON MENU");

        // Ternary operators to show the cursor ">"
        graphics_draw_text(disp, 100, 100, (selected_option == 0) ? "> START" : "  START");
        graphics_draw_text(disp, 100, 120, (selected_option == 1) ? "> EXIT" : "  EXIT");

        /* 4. Display and Handle Input */
        display_show(disp);

        controller_scan();
        struct controller_data keys = get_keys_down();

        if (keys.c[0].up)   selected_option = 0;
        if (keys.c[0].down) selected_option = 1;
        
        if (keys.c[0].A) {
            if(selected_option == 0) {
                /* Start Game Logic */
            }
        }
    }
}
