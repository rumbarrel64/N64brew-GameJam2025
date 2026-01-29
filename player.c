#include "player.h"
#include "physics.h"
#include "spores.h"

void player_init(Player *p) {
    //"catherine.blend" Model from: https://github.com/buu342/N64-Sausage64
    //License: WTFPL licen
    p->model = t3d_model_load("rom:/falloutGuy.t3dm");
    p->matrix = malloc_uncached(sizeof(T3DMat4FP));
    
    p->skel = t3d_skeleton_create(p->model);
    p->skelBlend = t3d_skeleton_clone(&p->skel, false);
    
    p->animIdle = t3d_anim_create(p->model, "Idle");
    t3d_anim_attach(&p->animIdle, &p->skel);
    
    p->animWalk = t3d_anim_create(p->model, "Walk");
    t3d_anim_attach(&p->animWalk, &p->skelBlend);
    
    p->animAttack = t3d_anim_create(p->model, "Attack1");
    t3d_anim_set_looping(&p->animAttack, false);
    t3d_anim_set_playing(&p->animAttack, false);
    t3d_anim_attach(&p->animAttack, &p->skel);

    p->position = (T3DVec3){{0, 0.15f, 0}};
    p->moveDir = (T3DVec3){{0, 0, 0}};
    p->rotY = 0.0f;
    p->currSpeed = 0.0f;
    p->isAttack = false;
    p->health = 5;
    p->repairPartsCount = 0;
}

void player_update(Player *p, float deltaTime, joypad_inputs_t joy, joypad_buttons_t btn, struct SporePlant *spores, int sporeCount) {
    // Attack Input
    if((btn.b) && !p->animAttack.isPlaying) {
        t3d_anim_set_playing(&p->animAttack, true);
        t3d_anim_set_time(&p->animAttack, 0.0f);
        p->isAttack = true;
    }

    // Movement calculation
    T3DVec3 stickDir = {{ (float)joy.stick_x * 0.05f, 0, -(float)joy.stick_y * 0.05f }};
    float speed = sqrtf(t3d_vec3_len2(&stickDir));

    if(speed > 0.15f && !p->isAttack) {
        stickDir.v[0] /= speed;
        stickDir.v[2] /= speed;
        p->moveDir = stickDir;

        float targetAngle = atan2f(p->moveDir.v[0], p->moveDir.v[2]);
        p->rotY = t3d_lerp_angle(p->rotY, targetAngle, 0.25f);
        p->currSpeed = t3d_lerp(p->currSpeed, speed * 0.15f, 0.15f);
    } else {
        p->currSpeed *= 0.8f;
    }

    p->animBlend = p->currSpeed / 0.51f;
    if(p->animBlend > 1.0f) p->animBlend = 1.0f;

    // Apply movement
    p->position.v[0] += p->moveDir.v[0] * p->currSpeed;
    p->position.v[2] += p->moveDir.v[2] * p->currSpeed;

    // Check Physics
    vault_check(&p->position, 140.0f, 110.0f);
    /*
    for(int i = 0; i < sporeCount; i++) {
        enemy_check(&p->position, spores[i].position, 40.0f);
    }
    */
    for(int i = 0; i < sporeCount; i++) {
        // Skip plants that are already "dead"
        if(!spores[i].active) continue;

        // A. Physical collision (Prevents walking through them)
        enemy_check(&p->position, spores[i].position, 40.0f);

    for(int i = 0; i < sporeCount; i++) {
        if(!spores[i].active) continue;

            // Physical collision (Prevents walking through them)
            enemy_check(&p->position, spores[i].position, 40.0f);

            // Radial Damage Check
            // If attacking and within the active hit frames
            if(p->isAttack && p->animAttack.time > 0.05f && p->animAttack.time < 0.40f) {
            
                float dx = spores[i].position.v[0] - p->position.v[0];
                float dz = spores[i].position.v[2] - p->position.v[2];
                float distSq = (dx * dx) + (dz * dz);

                // Distance check only (4900 = 70 units radius)
                if(distSq < 4900.0f) {
                    spores[i].health--;
                
                    // Jump animation time to 0.41f so it only triggers ONCE per swing
                    p->animAttack.time = 0.41f; 
                }
            }
        }
    }

    // Update Animations
    t3d_anim_update(&p->animIdle, deltaTime);
    t3d_anim_set_speed(&p->animWalk, p->animBlend + 0.15f);
    t3d_anim_update(&p->animWalk, deltaTime);

    if(p->isAttack) {
        t3d_anim_update(&p->animAttack, deltaTime);
        if(!p->animAttack.isPlaying) p->isAttack = false;
    }

    t3d_skeleton_blend(&p->skel, &p->skel, &p->skelBlend, p->animBlend);
    t3d_skeleton_update(&p->skel);

    // Build the matrix using the NEW, corrected position
    t3d_mat4fp_from_srt_euler(p->matrix,
        (float[3]){0.0035f, 0.0035f, 0.0035f},
        (float[3]){0.0f, -p->rotY, 0},
        p->position.v
    );
}

void player_draw(Player *p) {
    t3d_matrix_push(p->matrix);
    rdpq_set_prim_color(RGBA32(255, 255, 255, 255));
    t3d_model_draw_skinned(p->model, &p->skel);
    t3d_matrix_pop(1);
}

void player_cleanup(Player *p) {
    t3d_model_free(p->model);
    t3d_skeleton_destroy(&p->skel);
    t3d_skeleton_destroy(&p->skelBlend);
    t3d_anim_destroy(&p->animIdle);
    t3d_anim_destroy(&p->animWalk);
    t3d_anim_destroy(&p->animAttack);
    free_uncached(p->matrix);
}