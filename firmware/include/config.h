#ifndef CONFIG_H
#define CONFIG_H
/*
Various configuration parameters for the game, including pin assignments, and game settings.
*/


#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

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

#define COL_PORT PORTA
#define COL0_PIN PIN1_bm
#define COL1_PIN PIN2_bm
#define COL2_PIN PIN3_bm

#define ROW_PORT PORTA
#define ROW0_PIN PIN4_bm
#define ROW1_PIN PIN5_bm
#define ROW2_PIN PIN6_bm

#define DBG_UART_PORT PORTA
#define DBG_UART_TX_PIN PIN7_bm
#define DBG_UART_BAUD_RATE 9600u

#define NUM_ROWS 3
#define NUM_COLS 3
#define WINNING_NUM_BLINKS 4

#define BUTTON_SETTLE_US 80u
#define BUTTON_DEBOUNCE_SAMPLES 4u
#define BUTTON_SAMPLE_DELAY_MS 5u
#define WIN_BLINK_DELAY_MS 250u
#define US_PER_S 1000000UL
#define ALL_ON_MASK 0x01FFu
#define DEBUG_MODE 0u

#define GAME_INIT_RANDOM_MOVES 5u
#define TIMER_SEED_PRESC TCA_SINGLE_CLKSEL_DIV64_gc

#define DEBUG false

#endif
