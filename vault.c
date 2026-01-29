// vault.c
#include "vault.h"

void vault_init(Vault *vault, const char *model_path) {
    // Load model
    vault->model = t3d_model_load(model_path);

    // Default position
    vault->position = (T3DVec3){{0, 0, -10}};

    // Allocate matrix
    vault->matrix = malloc_uncached(sizeof(T3DMat4FP));

    // Computer Repairs
    vault->computerRepaired = false;

    // Default scale and rotation
    float scale[3] = {0.3f, 0.3f, 0.3f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};

    // Create transform matrix
    t3d_mat4fp_from_srt_euler(vault->matrix, scale, rotation, vault->position.v);
}

void vault_draw(Vault *vault) {
    t3d_matrix_push(vault->matrix);
    rdpq_set_prim_color(RGBA32(255, 255, 255, 255));
    t3d_model_draw(vault->model);
    t3d_matrix_pop(1);
}

void vault_cleanup(Vault *vault) {
    // Free model matrix
    free_uncached(vault->matrix);
    // Free model
    t3d_model_free(vault->model);
}