#include "game.h"

#include <avr/sleep.h>
#include <avr/interrupt.h>

static void go_to_sleep(void) {
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
        // sleep until an interrupt occurs from a button press
        go_to_sleep();
        game_update();
    }
    return 0;
}
