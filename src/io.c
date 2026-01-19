#include "io.h"
#include "config.h"
#include "helpers.h"

// -------------------------
// LED control
// -------------------------
inline void set_led(volatile PORT_t *p, uint8_t mask, bool on) {
    if (on) p->OUTSET = mask;
    else    p->OUTCLR = mask;
}

void io_leds_init(void) {
    // PB0..PB5 outputs
    PORTB.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;
    // PC0..PC2 outputs
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;
}

void io_leds_update(const uint16_t grid) {
    // Map grid bit N -> LEDN
    set_led(&LED0_PORT, LED0_PIN, (grid >> 0) & 1);
    set_led(&LED1_PORT, LED1_PIN, (grid >> 1) & 1);
    set_led(&LED2_PORT, LED2_PIN, (grid >> 2) & 1);
    set_led(&LED3_PORT, LED3_PIN, (grid >> 3) & 1);
    set_led(&LED4_PORT, LED4_PIN, (grid >> 4) & 1);
    set_led(&LED5_PORT, LED5_PIN, (grid >> 5) & 1);
    set_led(&LED6_PORT, LED6_PIN, (grid >> 6) & 1);
    set_led(&LED7_PORT, LED7_PIN, (grid >> 7) & 1);
    set_led(&LED8_PORT, LED8_PIN, (grid >> 8) & 1);
}

// -------------------------
// Button matrix scanning
// -------------------------
void io_buttons_init(void) {
    // Rows as outputs, idle HIGH
    ROW_PORT.DIRSET = ROW0_PIN | ROW1_PIN | ROW2_PIN;
    ROW_PORT.OUTSET = ROW0_PIN | ROW1_PIN | ROW2_PIN;

    // Cols as inputs with pullups
    COL_PORT.DIRCLR = COL0_PIN | COL1_PIN | COL2_PIN;

    // Enable pullups on PA1..PA3
    // Note: PINnCTRL corresponds to the pin number on that port.
    PORTA.PIN1CTRL = PORT_PULLUPEN_bm|PORT_ISC_FALLING_gc;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm|PORT_ISC_FALLING_gc;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm|PORT_ISC_FALLING_gc;
}

static inline void all_rows_high(void) {
    ROW_PORT.OUTSET = ROW0_PIN | ROW1_PIN | ROW2_PIN;
}

static inline void drive_row_low(uint8_t r) {
    all_rows_high();
    if (r == 0) ROW_PORT.OUTCLR = ROW0_PIN;
    if (r == 1) ROW_PORT.OUTCLR = ROW1_PIN;
    if (r == 2) ROW_PORT.OUTCLR = ROW2_PIN;
}

// Read columns: because of pullups, pressed reads LOW
static inline uint8_t read_cols_mask(void) {
    uint8_t in = COL_PORT.IN;
    uint8_t m = 0;
    if (!(in & COL0_PIN)) m |= (1u << 0);
    if (!(in & COL1_PIN)) m |= (1u << 1);
    if (!(in & COL2_PIN)) m |= (1u << 2);
    return m;
}

// Returns edges: bits that are newly pressed (debounced)
uint16_t io_buttons_scan(void) {
    static uint16_t prev_stable = 0;
    static uint16_t last_sample = 0;

    uint16_t sample = 0;

    for (uint8_t r = 0; r < NUM_ROWS; r++) {
        drive_row_low(r);
        _delay_us(80); // settle time

        uint8_t cols = read_cols_mask();
        for (uint8_t c = 0; c < NUM_COLS; c++) {
            if (cols & (1u << c)) {
                sample |= (1u << bit_index(r, c));
            }
        }
    }

    all_rows_high();

    // debounce: require the bit to appear in two consecutive scans
    uint16_t stable = sample & last_sample;
    last_sample = sample;

    uint16_t edges = stable & ~prev_stable;
    prev_stable = stable;

    return edges;
}