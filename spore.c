#include "spore.h"

static T3DModel *sporeModel = NULL;

void spore_init(SporePlant *plants, int count) {
    sporeModel = t3d_model_load("rom:/sporePlant.t3dm");

    for(int i = 0; i < count; i++) {
        plants[i].model = sporeModel;
        plants[i].matrix = malloc_uncached(sizeof(T3DMat4FP));
        
        plants[i].skel = t3d_skeleton_create(plants[i].model);
        plants[i].animIdle = t3d_anim_create(plants[i].model, "Idle");
        
        t3d_anim_attach(&plants[i].animIdle, &plants[i].skel);
        t3d_anim_set_playing(&plants[i].animIdle, true);
    }

    // Set specific positions for our 3 plants
    if(count >= 3) {
        plants[0].position = (T3DVec3){{ 50.0f, 0,  50.0f}};
        plants[1].position = (T3DVec3){{-75.0f, 0, 20.0f}}; // eft and down
        plants[2].position = (T3DVec3){{  0.0f, 0, -100.0f}}; // back wall
    }
}

void spore_update(SporePlant *plants, int count, float deltaTime, T3DVec3 playerPos) {
    for(int i = 0; i < count; i++) {
        
        // Update animations
        t3d_anim_update(&plants[i].animIdle, deltaTime);
        t3d_skeleton_update(&plants[i].skel);

        // Calculate distance to player (X and Z only)
        float dx = playerPos.v[0] - plants[i].position.v[0];
        float dz = playerPos.v[2] - plants[i].position.v[2];
        float distSq = (dx * dx) + (dz * dz);

        // Check if within radius (60 * 60 = 3600)
        if(distSq < 3600.0f) {
            // Calculate angle to look at player
            // atan2f returns the angle in radians
            //plants[i].rotY = -atan2f(dx, dz) + 4.7123f;

            // 1. Determine the target angle (where we WANT to face)
            float targetAngle = -atan2f(dx, dz) + 4.7123f;

            // 2. Find the difference between current and target
            float angleDiff = targetAngle - plants[i].rotY;

            // 3. Normalize the angle difference so the plant takes the shortest path
            // This prevents the plant from spinning 350 degrees the wrong way
            while (angleDiff < -3.14159f) angleDiff += 6.28318f;
            while (angleDiff >  3.14159f) angleDiff -= 6.28318f;

            // 4. Smoothly interpolate
            // turnSpeed: 1.0f is slow, 5.0f is fast. Adjust to your liking!
            float turnSpeed = 3.5f; 
            plants[i].rotY += angleDiff * turnSpeed * deltaTime;
        }

        // Build matrix for each plant
        t3d_mat4fp_from_srt_euler(plants[i].matrix,
            (float[3]){0.3f, 0.3f, 0.3f},
            (float[3]){0, plants[i].rotY, 0}, // Rotate on Y axis
            plants[i].position.v
        );
    }
}

void spore_draw(SporePlant *plants, int count) {
    for(int i = 0; i < count; i++) {
        t3d_matrix_push(plants[i].matrix);
        t3d_model_draw_skinned(plants[i].model, &plants[i].skel);
        t3d_matrix_pop(1);
    }
}

void spore_cleanup(SporePlant *plants, int count) {
    for(int i = 0; i < count; i++) {
        t3d_skeleton_destroy(&plants[i].skel);
        t3d_anim_destroy(&plants[i].animIdle);
        free_uncached(plants[i].matrix);
    }
    if(sporeModel) {
        t3d_model_free(sporeModel);
        sporeModel = NULL;
    }
}