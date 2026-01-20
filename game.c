#include "libs.h"
#include "gameState.h"
#include "player.h"
#include "vault.h"
#include "spore.h"

float get_time_s() {
  return (float)((double)get_ticks_us() / 1000000.0);
}

static bool initialized = false;
static T3DViewport viewport;
static T3DVec3 camPos, camTarget;
static T3DVec3 lightDirVec = {{1.0f, 1.0f, 1.0f}};
static rspq_syncpoint_t syncPoint = 0;
static float lastTime = 0;

// Object Variables
static Vault vault;
static Player player;
static SporePlant spores[3];

void play_loop() {

  // Get memory usage
heap_stats_t heap_stats;
sys_get_heap_stats(&heap_stats);
    
  if (!initialized) {
    
    // ======== 1. Intialize ======== //
        
        // Initialize Camera
        viewport = t3d_viewport_create();
        t3d_vec3_norm(&lightDirVec);
        
        // Initialize Vault
        vault_init(&vault, "rom:/vault.t3dm");

        // Initialize Player
        player_init(&player);

        // Initialize Spores
        spore_init(spores, 3);

        lastTime = get_time_s();
        initialized = true;
    }

    // ======== 2. Update ======== //
    joypad_poll();
    joypad_inputs_t joy = joypad_get_inputs(JOYPAD_PORT_1);
    joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    
    float now = get_time_s();
    float deltaTime = now - lastTime;
    if(deltaTime <= 0.0f || deltaTime > 0.1f) deltaTime = 0.0166f;
    lastTime = now;

    // Player Update
    player_update(&player, deltaTime, joy, btn, spores, 3);

    // Spore Update
    spore_update(spores, 3, deltaTime, player.position);

    // Update Camera
    camTarget = player.position;
    camTarget.v[2] -= 20;
    camPos.v[0] = camTarget.v[0];
    camPos.v[1] = camTarget.v[1] + 45;
    camPos.v[2] = camTarget.v[2] + 65;


     // ======== 3. Drawing ======== //   
     
     // Wait for RSP to finish previous frame
     if(syncPoint) rspq_syncpoint_wait(syncPoint);
    
     rdpq_attach(display_get(), display_get_zbuf());
     t3d_frame_start();
    
    // Set up viewport and camera
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 150.0f);
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});
    t3d_viewport_attach(&viewport);
    
    t3d_screen_clear_color(RGBA32(224, 180, 96, 255));
    t3d_screen_clear_depth();

    // Set up lighting
    t3d_light_set_ambient((uint8_t[4]){0xAA, 0xAA, 0xAA, 0xFF});
    t3d_light_set_directional(0, (uint8_t[4]){0xFF, 0xAA, 0xAA, 0xFF}, &lightDirVec);
    t3d_light_set_count(1);

    // Draw vault
    vault_draw(&vault);

    // Draw player
    player_draw(&player);

    // Draw Spore
    spore_draw(spores, 3);

    // Create sync point for next frame
    syncPoint = rspq_syncpoint_new();

    // ======== 4. Draw (UI) ======== //
      
    float posX = 16;
    float posY = 24;

    rdpq_sync_pipe();

    posY = 216;
    
    // MEMORY TRACKING
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 10, 15, "Mem: %d KiB", heap_stats.used/1024); // get memory usage
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "FPS: %.2f", display_get_fps()); posY += 10; // Get FPS

    // PLAYER
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, posX, posY, "Player Pos. (X, Y): (%.4f, %.4f)", player.position.v[0], player.position.v[2]); posY += 10; //Displays position

    rdpq_detach_show();

     // ======== 5. Game Exit and Cleanup ======== //
    if(btn.start) {

            // Cleanup
            player_cleanup(&player);
            vault_cleanup(&vault);
            spore_cleanup(spores, 3);
            initialized = false;
            state = STATE_MENU;
        }
    }