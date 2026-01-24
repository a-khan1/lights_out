#ifndef IO_H
#define IO_H
#include "config.h"
#include "helpers.h"

#define US_PER_S 1000000UL

void io_leds_init(void);
void io_leds_update(const uint16_t grid);
void io_buttons_init(void);
uint16_t io_buttons_scan(void);
void io_buttons_prepare_sleep(void);
void io_debug_uart_init(void);
void io_debug_uart_write_byte(uint8_t byte);
void io_debug_uart_write_str(const char *s);

#endif
