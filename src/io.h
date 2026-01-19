#ifndef IO_H
#define IO_H
#include "config.h"
#include "helpers.h"

void io_leds_init(void);
void io_leds_update(const uint16_t grid);
void io_buttons_init(void);
uint16_t io_buttons_scan(void);

#endif // IO_H