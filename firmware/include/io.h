#ifndef IO_H
#define IO_H
/* 
I/O module for LED control, button matrix scanning, and debug UART.
*/
#include "config.h"

void io_leds_init(void);
void io_leds_update(const uint16_t grid);
void io_buttons_init(void);
uint16_t io_buttons_read(void);
void io_debug_uart_init(void);
void io_debug_uart_write(const uint8_t *buf, uint16_t len);

#endif
