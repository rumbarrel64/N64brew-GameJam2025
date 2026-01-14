// vault.h
#pragma once

#include "libs.h"

typedef struct {
    T3DModel *model;
    T3DMat4FP *matrix;
    T3DVec3 position;
} Vault;

void vault_init(Vault *vault, const char *model_path);
void vault_draw(Vault *vault);
void vault_cleanup(Vault *vault);