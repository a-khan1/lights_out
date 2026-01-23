#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// LEDs (9 outputs)
#define LED0_PORT PORTB
#define LED0_PIN  PIN0_bm
#define LED1_PORT PORTB
#define LED1_PIN  PIN1_bm
#define LED2_PORT PORTB
#define LED2_PIN  PIN2_bm
#define LED3_PORT PORTB
#define LED3_PIN  PIN3_bm
#define LED4_PORT PORTB
#define LED4_PIN  PIN4_bm
#define LED5_PORT PORTB
#define LED5_PIN  PIN5_bm

#define LED6_PORT PORTC
#define LED6_PIN  PIN0_bm
#define LED7_PORT PORTC
#define LED7_PIN  PIN1_bm
#define LED8_PORT PORTC
#define LED8_PIN  PIN2_bm

// Button matrix: COL inputs (pullup), ROW outputs
#define COL_PORT PORTA
#define COL0_PIN PIN1_bm  // PA1
#define COL1_PIN PIN2_bm  // PA2
#define COL2_PIN PIN3_bm  // PA3

#define ROW_PORT PORTA
#define ROW0_PIN PIN4_bm  // PA4
#define ROW1_PIN PIN5_bm  // PA5
#define ROW2_PIN PIN6_bm  // PA6

#define NUM_ROWS 3
#define NUM_COLS 3
#define WINNING_NUM_BLINKS 6
#define WINNING_GAME_STATE 0x0000  // all LEDs off

#endif // CONFIG_H