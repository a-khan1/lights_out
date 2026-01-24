#include "config.h"
#include "game.h"

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void go_to_sleep(void) {
    set_sleep_mode(SLEEP_MODE_STANDBY);
    sleep_enable();
    PORTA.INTFLAGS = 0xFF;
    sei();
    sleep_cpu();
    sleep_disable();
}

ISR(PORTA_PORT_vect) {
    PORTA.INTFLAGS = 0xFF;
}

int main(void) {
    game_init();

    while (1){
        io_buttons_prepare_sleep();
        go_to_sleep();

        uint16_t edges = 0;
        for (uint8_t i = 0; i < BUTTON_DEBOUNCE_SAMPLES; i++) {
            edges |= game_scan_buttons_edges();
            _delay_ms(BUTTON_SAMPLE_DELAY_MS);
        }
        if (edges) {
            for (uint8_t r = 0; r < NUM_ROWS; r++) {
                for (uint8_t c = 0; c < NUM_COLS; c++) {
                    if (edges & (1u << grid_index(r, c))) {
                        game_apply_move(r, c);
                    }
                }
            }
            if (game_solved()) {
                game_win_blink();
            }
        }
        _delay_ms(BUTTON_POST_SCAN_DELAY_MS);
    }
    return 0;
}
