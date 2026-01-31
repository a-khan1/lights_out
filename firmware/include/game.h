#ifndef GAME_H
#define GAME_H
/* 
Game logic and state management for Lights Out game.
*/
#include "io.h"

#define WINNING_GAME_STATE 0x0000u

void game_init(void);
void game_update(void);

#endif
