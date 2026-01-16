#ifndef GAME_H
#define GAME_H
#include "io.h"

void game_init(void);
void win_blink(void);
uint16_t scan_buttons_edges(void);
void go_to_sleep(void);
uint8_t solved(void);

#endif // GAME_H