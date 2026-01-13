#include "libs.h"
#include "gameState.h"

float get_time_s() {
  return (float)((double)get_ticks_us() / 1000000.0);
}

static bool initialized = false;
static T3DViewport viewport;
static T3DMat4FP *modelMatFP;
static T3DMat4FP *mapMatFP;
static T3DVec3 camPos;
static T3DVec3 camTarget;
static T3DModel *model = NULL;
static T3DModel *modelMap = NULL;
static T3DSkeleton skel;
static T3DSkeleton skelBlend;
static T3DAnim animIdle;
static T3DAnim animWalk;
static T3DAnim animAttack;
static float lastTime = 0;
static float rotY = 0.0f;
static T3DVec3 playerPos = {{0, 0.15f, 0}};
static T3DVec3 moveDir = {{0, 0, 0}};
static T3DVec3 lightDirVec = {{1.0f, 1.0f, 1.0f}};
static rspq_syncpoint_t syncPoint = 0;
static float currSpeed = 0.0f;
static float animBlend = 0.0f;
static bool isAttack = false;

void play_loop() {
    if (!initialized) {
        viewport = t3d_viewport_create();
        
        // Normalize light direction
        t3d_vec3_norm(&lightDirVec);
        
        // Set up camera position
        camPos = (T3DVec3){{0, 45.0f, 80.0f}};
        camTarget = (T3DVec3){{0, 0, 0}};
        
        modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
        mapMatFP = malloc_uncached(sizeof(T3DMat4FP));
        
        // Set up map matrix
        t3d_mat4fp_from_srt_euler(mapMatFP, 
            (float[3]){0.3f, 0.3f, 0.3f}, 
            (float[3]){0, 0, 0}, 
            (float[3]){0, 0, -10}
        );

        // Load models
        model = t3d_model_load("rom:/falloutGuy.t3dm");
        modelMap = t3d_model_load("rom:/vault.t3dm");
        
        if(model == NULL) {
            debugf("ERROR: Failed to load falloutGuy.t3dm\n");
            return; 
        }
        
        if(modelMap == NULL) {
            debugf("ERROR: Failed to load vault.t3dm\n");
        }
        
        // Create skeletons
        skel = t3d_skeleton_create(model);
        skelBlend = t3d_skeleton_clone(&skel, false);
        
        // Create animation instances
        animIdle = t3d_anim_create(model, "Idle");
        t3d_anim_attach(&animIdle, &skel);
        
        animWalk = t3d_anim_create(model, "Walk");
        t3d_anim_attach(&animWalk, &skelBlend);
        
        animAttack = t3d_anim_create(model, "Attack1");
        t3d_anim_set_looping(&animAttack, false);
        t3d_anim_set_playing(&animAttack, false);
        t3d_anim_attach(&animAttack, &skel);

        lastTime = get_time_s();
        initialized = true;
    }

    // ======== 1. Update ======== //
    joypad_poll();
    joypad_inputs_t joy = joypad_get_inputs(JOYPAD_PORT_1);
    joypad_buttons_t btn = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    
    float now = get_time_s();
    float deltaTime = now - lastTime;
    if(deltaTime <= 0.0f || deltaTime > 0.1f) deltaTime = 0.0166f;
    lastTime = now;

    // Movement calculation
    T3DVec3 newDir = {{
       (float)joy.stick_x * 0.05f, 0,
      -(float)joy.stick_y * 0.05f
    }};
    
    float speed = sqrtf(t3d_vec3_len2(&newDir));

    // Player Attack
    if((btn.a || btn.b) && !animAttack.isPlaying) {
      t3d_anim_set_playing(&animAttack, true);
      t3d_anim_set_time(&animAttack, 0.0f);
      isAttack = true;
    }

    // Player movement
    if(speed > 0.15f && !isAttack) {
      newDir.v[0] /= speed;
      newDir.v[2] /= speed;
      moveDir = newDir;

      float newAngle = atan2f(moveDir.v[0], moveDir.v[2]);
      rotY = t3d_lerp_angle(rotY, newAngle, 0.25f);
      currSpeed = t3d_lerp(currSpeed, speed * 0.15f, 0.15f);
    } else {
      currSpeed *= 0.8f;
    }

    // Use blend based on speed for smooth transitions
    animBlend = currSpeed / 0.51f;
    if(animBlend > 1.0f)animBlend = 1.0f;

    // Move player
    playerPos.v[0] += moveDir.v[0] * currSpeed;
    playerPos.v[2] += moveDir.v[2] * currSpeed;

    // Update camera to follow player
    camTarget = playerPos;
    camTarget.v[2] -= 20;
    camPos.v[0] = camTarget.v[0];
    camPos.v[1] = camTarget.v[1] + 45;
    camPos.v[2] = camTarget.v[2] + 65;

    // Update animations
    t3d_anim_update(&animIdle, deltaTime);
    t3d_anim_set_speed(&animWalk, animBlend + 0.15f);
    t3d_anim_update(&animWalk, deltaTime);

    if(isAttack) {
      t3d_anim_update(&animAttack, deltaTime);
      if(!animAttack.isPlaying)isAttack = false;
    }

    // Blend animations
    t3d_skeleton_blend(&skel, &skel, &skelBlend, animBlend);
    
    // Wait for RSP to finish previous frame
    if(syncPoint) rspq_syncpoint_wait(syncPoint);
    
    // Update skeleton matrices
    t3d_skeleton_update(&skel);

    // Update player matrix
    t3d_mat4fp_from_srt_euler(modelMatFP,
      (float[3]){0.0035f, 0.0035f, 0.0035f},
      (float[3]){0.0f, -rotY, 0},
      playerPos.v
    );

    // ======== 2. Drawing ======== //
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

    // Draw vault map first (background)
    if(modelMap != NULL) {
        t3d_matrix_push(mapMatFP);
        rdpq_set_prim_color(RGBA32(255, 255, 255, 255));
        t3d_model_draw(modelMap);
        t3d_matrix_pop(1);
    }

    // Draw player model
    t3d_matrix_push(modelMatFP);
      rdpq_set_prim_color(RGBA32(255, 255, 255, 255));
      t3d_model_draw_skinned(model, &skel);
    t3d_matrix_pop(1);

    // Create sync point for next frame
    syncPoint = rspq_syncpoint_new();

    rdpq_detach_show();

    if(btn.start) state = STATE_MENU;
}