// tutorial.c
// General file to hold all libraries
#include "libs.h"
#include "gameState.h"

void play_loop() {

  // Initialize Camera
  // Initialize Camera
  
  // Initialize Map(s)

  // Initialize Map(s)

  // Initialize Player

  // Initialize Player

    while (state ==  STATE_GAME) {

      //The joypad subsystem only polls the controllers once per VI interrupt.
      joypad_poll();

      joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);
 

      // ======== Draw (UI) ======== //


      // MEMORY TRACKING
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 10, 15, "Mem: %d KiB", heap_stats.used/1024); // get memory usage
     // rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "FPS: %.2f", display_get_fps()); posY += 10; // Get FPS
  
      // LEVEL
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Map (X, Y): (%.4f, %.4f)",  map.position.v[0], map.position.v[2]); posY += 10; //Displays position
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Positions (X, Y): (%d, %d)", joypad.stick_x, joypad.stick_y); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Enemy Count: (%d)", enemy_count); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Total Levels: (%d)", TOTAL_LEVELS); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Current Level: (%d)", get_current_level_index() + 1); posY += 10;

      // BULLET
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Bullet Position (X, Y): (%.4f, %.4f)", bullet.position.v[0], bullet.position.v[2]); posY += 10; //Displays position
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Bullet Rotation: %.4f", bullet.rotation_y); posY += 10;

      //TIME
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Game Time: (%.4f)", get_time_s()); posY += 10; // Since turning on n64
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Level Start Time: (%.4f)", level_timer); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Level Start Time: (%.4f)", tutorial_time); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "After Death time: (%.4f)", get_time_s() - zombies[0].blood_time); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Reset Time: (%.4f)", get_time_s() - level_timer); posY += 10;
      

      //SLAYER
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Zombie Player Distance (X, Y): (%.4f)", sqrt((player.position.v[0] - zombies[0].position.v[0]) * (player.position.v[0] - zombies[0].position.v[0]) + (player.position.v[2] - zombies[0].position.v[2]) * (player.position.v[2] - zombies[0].position.v[2]))); posY += 10; //Displays position
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Slayer Pos. (X, Y, Z): (%.4f, %.4f, %.4f)", player.position.v[0], player.position.v[2], player.position.v[1]); posY += 10; //Displays position
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Slayer Pos. (X, Y): (%.4f, %.4f)", player.position.v[0], player.position.v[2]); posY += 10; //Displays position
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Slayer Rotation (Y):%.4f", player.rotation_y); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Speed: %.4f", player.speed); posY += 10; //Speed

      // ZOMBIE
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Zombie blood scale: (%.4f)",zombies[1].blood_scale - 0.01f * (get_time_s() - zombies[0].blood_time)); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Zombie Count: (%d)", zombie_count); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Enemy Count: (%d)", enemy_count); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Zombie Death time:%.4f", zombies[0].blood_time); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Zombie Life: (%d, %d)", zombies[0].health, zombies[1].health); posY += 10;
      //rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Zombie Alive: (%d, %d)", zombies[0].alive, zombies[1].alive); posY += 10;


      rdpq_detach_show();

    // Manual exit - press start OR reached end level
    if (btn.start) {
        state = STATE_MENU;
    };
  
    }; // End Tutorial Loop

    // Map Cleanup
    
};