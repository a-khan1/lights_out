/*
  Lights Out game for ATTiny1616
  3x3 grid of buttons and LEDs
  Pressing a button toggles that LED and its orthogonal neighbors
  Goal: turn off all LEDs
*/

#include "config.h"
#include "game.h"

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void go_to_sleep(void) {
    set_sleep_mode(SLEEP_MODE_STANDBY);
    sleep_enable();
    // Clear any latched pin-change flags before sleeping
    PORTA.INTFLAGS = 0xFF;
    sei();              // global interrupts ON
    sleep_cpu();        // sleep here
    sleep_disable();    // resumes here after wake
}

// Wake on PORTA pin-change (adjust if you use a different port for wake)
ISR(PORTA_PORT_vect) {
    // Clear all PORTA interrupt flags
    PORTA.INTFLAGS = 0xFF;
}

int main(void) {
    // Start game
    game_init();

    while (1) {
        // sleep until button interrupt
        go_to_sleep();

        uint16_t edges = 0;
        for (uint8_t i = 0; i < 4; i++) {
            edges |= game_scan_buttons_edges();
            _delay_ms(5);
        }
        if (edges) {
            for (uint8_t r = 0; r < NUM_ROWS; r++) {
                for (uint8_t c = 0; c < NUM_COLS; c++) {
                    if (edges & (1u << bit_index(r, c))) {
                        game_apply_move(r, c);
                    }
                }
            }
            if (game_solved()) {
                game_win_blink();   // game.c will blink + restart
            }
        }
        // 20ms debounce
        _delay_ms(20);
    }
    return 0;
}
