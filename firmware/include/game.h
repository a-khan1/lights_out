#ifndef GAME_H
#define GAME_H
#include "io.h"

void game_init(void);
void game_win_blink(void);
uint16_t game_scan_buttons_edges(void);
uint8_t game_solved(void);
void game_apply_move(uint8_t r, uint8_t c);

#endif // GAME_H