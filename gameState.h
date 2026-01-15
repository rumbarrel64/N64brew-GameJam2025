#ifndef GAMESTATE_H
#define GAMESTATE_H

typedef enum {
    STATE_MENU,
    STATE_TUTORIAL,
    STATE_GAME
} GameState;

extern GameState state;

#endif