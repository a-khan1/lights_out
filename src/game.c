#include "game.h"
#include <util/delay.h>
// -------------------------
// Game state and logic
// -------------------------

static uint16_t grid = 0; // bits 0..8 represent LEDs 0..8

uint16_t game_get_grid(void) {
    return grid;
}

static inline void toggle_cell(uint8_t r, uint8_t c) {
    if (r >= NUM_ROWS || c >= NUM_COLS) return;
    grid ^= (1u << bit_index(r, c));
}

void apply_move(uint8_t r, uint8_t c) {
    // Toggle the cell and its orthogonal neighbors
    toggle_cell(r, c);
    if (r > 0) toggle_cell(r - 1, c);
    if (r < (NUM_ROWS - 1)) toggle_cell(r + 1, c);
    if (c > 0) toggle_cell(r, c - 1);
    if (c < (NUM_COLS - 1)) toggle_cell(r, c + 1);
}

void game_init(void) {
    // Simple guaranteed-solvable start: apply a few moves
    leds_init();
    buttons_init();
    grid = 0;
    apply_move(0, 0);
    apply_move(0, 2);
    apply_move(1, 1);
    apply_move(2, 0);
    apply_move(2, 2);
    leds_update(game_get_grid());
}

uint8_t solved(void) {
    // Only the lower 9 bits matter
    return (grid & 0x01FFu) == 0;
}

void win_blink(void) {
    for (uint8_t i = 0; i < WINNING_NUM_BLINKS; i++) {
        grid = 0x01FFu;
        leds_update(grid);
        _delay_ms(120);

        grid = 0;
        leds_update(grid);
        _delay_ms(120);
    }

    game_init();
    // (Don’t call leds_update here if you prefer main to do it;
    // leaving it is fine either way. If you want main to handle all display,
    // just remove the next line.)
    leds_update(grid);
}
