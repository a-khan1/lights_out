#include "game.h"
#include <util/delay.h>
static uint16_t grid = WINNING_GAME_STATE; 

static inline uint8_t grid_index(uint8_t r, uint8_t c) {
    return (uint8_t)(r * NUM_COLS + c);
}

static inline void toggle_cell(uint8_t r, uint8_t c) {
    if (r >= NUM_ROWS || c >= NUM_COLS) return;
    grid ^= (1u << grid_index(r, c));
}

static void timer_init(void) {
    TCA0.SINGLE.CTRLB = 0;
    TCA0.SINGLE.CNT = 0;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TIMER_SEED_PRESC;
}

// pseudo random number generator using timer counter
static uint16_t random_generate(void) {
    return TCA0.SINGLE.CNT;
}


static void apply_move(uint8_t r, uint8_t c) {
    toggle_cell(r, c);
    if (r > 0) toggle_cell(r - 1, c);
    if (r < (NUM_ROWS - 1)) toggle_cell(r + 1, c);
    if (c > 0) toggle_cell(r, c - 1);
    if (c < (NUM_COLS - 1)) toggle_cell(r, c + 1);
    io_leds_update(grid);
}

static uint8_t solved(void) {
    return grid == WINNING_GAME_STATE;
}

static void win_blink(void) {
    for (uint8_t i = 0; i < WINNING_NUM_BLINKS; i++) {
        grid = ALL_ON_MASK;
        io_leds_update(grid);
        _delay_ms(WIN_BLINK_DELAY_MS);

        grid = WINNING_GAME_STATE;
        io_leds_update(grid);
        _delay_ms(WIN_BLINK_DELAY_MS);
    }

    game_init();
}

void game_init(void) {
    io_leds_init();
    io_buttons_init();
    #if DEBUG
    // Startup LED test: toggle each LED in order (LED0 -> LED8).
    io_leds_update(grid);
    grid = WINNING_GAME_STATE;
    for (uint8_t i = 0; i < (uint8_t)(NUM_ROWS * NUM_COLS); i++) {
        grid = (uint16_t)(1u << i);
        io_leds_update(grid);
        _delay_ms(200);
    }
    #endif

    grid = WINNING_GAME_STATE;
    timer_init();
    uint16_t seed = random_generate();
    for (uint8_t i = 0; i < GAME_INIT_RANDOM_MOVES; i++) {
        uint8_t r = (uint8_t)((seed + i) % NUM_ROWS);
        uint8_t c = (uint8_t)(((seed >> 8) + i) % NUM_COLS);
        apply_move(r, c);
    }
    uint16_t extra = 0;
    // if the puzzle is solved, unsolve it by applying an extra move
    if (solved()) {
        uint8_t r = (uint8_t)((seed + extra) % NUM_ROWS);
        uint8_t c = (uint8_t)(((seed >> 8) + extra) % NUM_COLS);
        apply_move(r, c);
        extra++;
    }
    io_leds_update(grid);
}

void game_update(void) {
    io_sleep_until_button();
    // capture button input
    uint16_t pressed_mask = io_buttons_read();
    // use button input to update game state
    if (pressed_mask) {
        for (uint8_t r = 0; r < NUM_ROWS; r++) {
            for (uint8_t c = 0; c < NUM_COLS; c++) {
                if (pressed_mask & (1u << grid_index(r, c))) {
                    apply_move(r, c);
                }
            }
        }
        // check for win condition
        if (solved()) {
            win_blink();
        }
    }
    _delay_ms(BUTTON_SAMPLE_DELAY_MS);
}
