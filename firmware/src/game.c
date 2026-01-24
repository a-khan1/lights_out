#include "game.h"
#include <util/delay.h>
static uint16_t grid = WINNING_GAME_STATE; 
static const uint8_t init_moves[][2] = {
    {0u, 0u},
    {0u, (uint8_t)(NUM_COLS - 1u)},
    {(uint8_t)(NUM_ROWS / 2u), (uint8_t)(NUM_COLS / 2u)},
    {(uint8_t)(NUM_ROWS - 1u), 0u},
    {(uint8_t)(NUM_ROWS - 1u), (uint8_t)(NUM_COLS - 1u)},
};

static const uint16_t all_on_mask =
    (uint16_t)((1u << (NUM_ROWS * NUM_COLS)) - 1u);

static inline void toggle_cell(uint8_t r, uint8_t c) {
    if (r >= NUM_ROWS || c >= NUM_COLS) return;
    grid ^= (1u << grid_index(r, c));
}

void game_apply_move(uint8_t r, uint8_t c) {
    toggle_cell(r, c);
    if (r > 0) toggle_cell(r - 1, c);
    if (r < (NUM_ROWS - 1)) toggle_cell(r + 1, c);
    if (c > 0) toggle_cell(r, c - 1);
    if (c < (NUM_COLS - 1)) toggle_cell(r, c + 1);
    io_leds_update(grid);
}

// TODO: implement randomization of game state at initialization
void game_init(void) {
    io_leds_init();
    io_buttons_init();
    grid = WINNING_GAME_STATE;
    for (uint8_t i = 0; i < (uint8_t)(sizeof(init_moves) / sizeof(init_moves[0])); i++) {
        game_apply_move(init_moves[i][0], init_moves[i][1]);
    }
    io_leds_update(grid);
}

uint16_t game_scan_buttons_edges(void) {
    return io_buttons_scan();
}

uint8_t game_solved(void) {
    return grid == WINNING_GAME_STATE;
}

void game_win_blink(void) {
    for (uint8_t i = 0; i < WINNING_NUM_BLINKS; i++) {
        grid = all_on_mask;
        io_leds_update(grid);
        _delay_ms(WIN_BLINK_DELAY_MS);

        grid = WINNING_GAME_STATE;
        io_leds_update(grid);
        _delay_ms(WIN_BLINK_DELAY_MS);
    }

    game_init();
}
