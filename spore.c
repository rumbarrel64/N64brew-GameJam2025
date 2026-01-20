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
        plants[1].position = (T3DVec3){{-50.0f, 0, -80.0f}};
        plants[2].position = (T3DVec3){{  0.0f, 0, -100.0f}};
    }
}

void spore_update(SporePlant *plants, int count, float deltaTime) {
    for(int i = 0; i < count; i++) {
        t3d_anim_update(&plants[i].animIdle, deltaTime);
        t3d_skeleton_update(&plants[i].skel);

        // Build matrix for each plant
        t3d_mat4fp_from_srt_euler(plants[i].matrix,
            (float[3]){0.3f, 0.3f, 0.3f},
            (float[3]){0, 0, 0},
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