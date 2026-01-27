#include "spores.h"
#include "player.h"
#include "physics.h"
#include <math.h>

static T3DModel *sporePlantModel = NULL;
static T3DModel *sporeBulletModel = NULL;

void spores_init(SporePlant *plants, int count) {
    // Load models once for all plants to share
    sporePlantModel = t3d_model_load("rom:/sporePlant.t3dm");
    sporeBulletModel = t3d_model_load("rom:/spore.t3dm");

    for(int i = 0; i < count; i++) {
        
        // Spore Plant
        plants[i].model = sporePlantModel;
        plants[i].matrix = malloc_uncached(sizeof(T3DMat4FP));
        plants[i].skel = t3d_skeleton_create(plants[i].model);
        plants[i].animIdle = t3d_anim_create(plants[i].model, "Idle");       
        t3d_anim_attach(&plants[i].animIdle, &plants[i].skel);
        t3d_anim_set_playing(&plants[i].animIdle, true);
        plants[i].rotY = 0.0f;
        plants[i].shootTimer = 0.0f;

        plants[i].health = 3;
        plants[i].active = true;

        // Positions
        if(count >= 3) {
            plants[0].position = (T3DVec3){{ 50.0f, 0,  50.0f}};
            plants[1].position = (T3DVec3){{-75.0f, 0,  20.0f}};
            plants[2].position = (T3DVec3){{  0.0f, 0, -100.0f}};
        }

        // Spore 
        plants[i].bullet.model = sporeBulletModel;
        plants[i].bullet.matrix = malloc_uncached(sizeof(T3DMat4FP));
        plants[i].bullet.active = false;
        plants[i].bullet.rotY = 0.0f;
    }
}

void spores_update(SporePlant *plants, int count, float deltaTime, Player *player) {
    for(int i = 0; i < count; i++) {
        
        // Check if plant is dead
        if(!plants[i].active) continue;

        if(plants[i].health <= 0) {
            plants[i].active = false;
            plants[i].bullet.active = false; // Kill the bullet too when plant dies
            continue;
        }
        
        // 1. Update plant animations
        t3d_anim_update(&plants[i].animIdle, deltaTime);
        t3d_skeleton_update(&plants[i].skel);

        // 2. Handle Rotation
        float dx = player->position.v[0] - plants[i].position.v[0];
        float dz = player->position.v[2] - plants[i].position.v[2];
        float distSq = (dx * dx) + (dz * dz);

        // Only rotate and shoot if player is in range (60 units)
        if(distSq < 3600.0f) {
            // Smoothly rotate to face player
            float targetAngle = -atan2f(dx, dz) + 4.7123f;
            float angleDiff = targetAngle - plants[i].rotY;
            
            while (angleDiff < -3.14159f) angleDiff += 6.28318f;
            while (angleDiff >  3.14159f) angleDiff -= 6.28318f;
            
            plants[i].rotY += angleDiff * 3.5f * deltaTime;

            // 3. Handle Firing logic
            plants[i].shootTimer += deltaTime;
            if(plants[i].shootTimer > 2.0f && !plants[i].bullet.active) {
                plants[i].bullet.active = true;
                plants[i].bullet.position = plants[i].position;
                plants[i].bullet.position.v[1] = 15.0f; // Height of plant's mouth
                // Spore Direction
                float dist = sqrtf(distSq);
                plants[i].bullet.direction.v[0] = (dx / dist) * 2.5f; // Projectile Speed
                plants[i].bullet.direction.v[2] = (dz / dist) * 2.5f;
                // Spore Rotation
                plants[i].bullet.rotY = -atan2f(plants[i].bullet.direction.v[0], plants[i].bullet.direction.v[2]);
                
                plants[i].shootTimer = 0.0f;
            }
        }

        // 4. Update Projectile (Bullet) if it's flying
        if(plants[i].bullet.active) {
            plants[i].bullet.position.v[0] += plants[i].bullet.direction.v[0];
            plants[i].bullet.position.v[2] += plants[i].bullet.direction.v[2];

            // Check collision with player (radius 15)
            if(projectile_check(plants[i].bullet.position, player->position, 15.0f)) {
                if(player->health > 0) player->health--;     // Take damage
                    plants[i].bullet.active = false;         // Destroy bullet
            }

            // Despawn bullet if it flies too far away
            if(distSq > 40000.0f) plants[i].bullet.active = false;

            // Update Bullet Matrix
            t3d_mat4fp_from_srt_euler(plants[i].bullet.matrix,
                (float[3]){0.035f, 0.035f, 0.035f}, // Bullet scale
                (float[3]){0, plants[i].bullet.rotY, 0},
                plants[i].bullet.position.v
            );
        }

        // Update Plant Matrix
        t3d_mat4fp_from_srt_euler(plants[i].matrix,
            (float[3]){0.3f, 0.3f, 0.3f}, // Plant scale
            (float[3]){0, plants[i].rotY, 0},
            plants[i].position.v
        );
    }
}

void spores_draw(SporePlant *plants, int count) {
    for(int i = 0; i < count; i++) {
        // Only draw if plant is alive 
        if(!plants[i].active) continue;

        // Draw the Plant
        t3d_matrix_push(plants[i].matrix);
        t3d_model_draw_skinned(plants[i].model, &plants[i].skel);
        t3d_matrix_pop(1);

        // Draw the Bullet (if it's active)
        if(plants[i].bullet.active) {
            t3d_matrix_push(plants[i].bullet.matrix);
            t3d_model_draw(plants[i].bullet.model);
            t3d_matrix_pop(1);
        }
    }
}

void spores_cleanup(SporePlant *plants, int count) {
    for(int i = 0; i < count; i++) {
        t3d_skeleton_destroy(&plants[i].skel);
        t3d_anim_destroy(&plants[i].animIdle);
        free_uncached(plants[i].matrix);
        free_uncached(plants[i].bullet.matrix);
    }
    if(sporePlantModel) t3d_model_free(sporePlantModel);
    if(sporeBulletModel) t3d_model_free(sporeBulletModel);
}