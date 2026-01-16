#ifndef IO_H
#define IO_H
#include "config.h"
#include "helpers.h"

void leds_init(void);
void leds_update(const uint16_t grid);
void buttons_init(void);
uint16_t scan_buttons_edges(void);

#endif // IO_H