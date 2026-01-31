#include "io.h"
#include "config.h"

static inline uint8_t grid_index(uint8_t r, uint8_t c) {
    return (uint8_t)(r * NUM_COLS + c);
}

static inline void set_led(volatile PORT_t *p, uint8_t mask, bool on) {
    if (on) p->OUTSET = mask;
    else    p->OUTCLR = mask;
}

void io_leds_init(void) {
    PORTB.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;
    PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;
}

void io_leds_update(const uint16_t grid) {
    set_led(&LED0_PORT, LED0_PIN, (grid >> 0) & 1u);
    set_led(&LED1_PORT, LED1_PIN, (grid >> 1) & 1u);
    set_led(&LED2_PORT, LED2_PIN, (grid >> 2) & 1u);
    set_led(&LED3_PORT, LED3_PIN, (grid >> 3) & 1u);
    set_led(&LED4_PORT, LED4_PIN, (grid >> 4) & 1u);
    set_led(&LED5_PORT, LED5_PIN, (grid >> 5) & 1u);
    set_led(&LED6_PORT, LED6_PIN, (grid >> 6) & 1u);
    set_led(&LED7_PORT, LED7_PIN, (grid >> 7) & 1u);
    set_led(&LED8_PORT, LED8_PIN, (grid >> 8) & 1u);
}


void io_buttons_init(void) {
    COL_PORT.DIRSET = COL0_PIN | COL1_PIN | COL2_PIN;
    COL_PORT.OUTSET = COL0_PIN | COL1_PIN | COL2_PIN;

    ROW_PORT.DIRCLR = ROW0_PIN | ROW1_PIN | ROW2_PIN;
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;

}

static inline void all_cols_high(void) {
    COL_PORT.OUTSET = COL0_PIN | COL1_PIN | COL2_PIN;
}

static inline void drive_col_low(uint8_t c) {
    all_cols_high();
    if (c == 0u) COL_PORT.OUTCLR = COL0_PIN;
    if (c == 1u) COL_PORT.OUTCLR = COL1_PIN;
    if (c == 2u) COL_PORT.OUTCLR = COL2_PIN;
}

static inline uint8_t read_rows_mask(void) {
    uint8_t in = ROW_PORT.IN;
    uint8_t m = 0;
    if (!(in & ROW0_PIN)) m |= (1u << 0);
    if (!(in & ROW1_PIN)) m |= (1u << 1);
    if (!(in & ROW2_PIN)) m |= (1u << 2);
    return m;
}

static uint16_t buttons_read_raw(void) {
    uint16_t sample = 0;

    for (uint8_t c = 0; c < NUM_COLS; c++) {
        drive_col_low(c);
        _delay_us(BUTTON_SETTLE_US);

        uint8_t rows = read_rows_mask();
        for (uint8_t r = 0; r < NUM_ROWS; r++) {
            if (rows & (1u << r)) {
                sample |= (1u << grid_index(r, c));
            }
        }
    }

    all_cols_high();
    return sample;
}

uint16_t io_buttons_read(void) {
    static uint16_t stable = 0;
    static uint8_t stable_count = 0;
    static uint8_t latched = 0;

    uint16_t sample = buttons_read_raw();
    if (sample == stable) {
        if (stable_count < BUTTON_DEBOUNCE_SAMPLES) {
            stable_count++;
        }
    } else {
        stable = sample;
        stable_count = 0;
    }

    uint16_t edges = 0;
    if (!latched && stable && stable_count >= (BUTTON_DEBOUNCE_SAMPLES - 1)) {
        latched = 1;
        edges = stable;
    }
    if (latched && stable == 0 && stable_count >= (BUTTON_DEBOUNCE_SAMPLES - 1)) {
        latched = 0;
    }

    return edges;
}

// Software UART implementation for debugging
void io_debug_uart_init(void) {
    DBG_UART_PORT.DIRSET = DBG_UART_TX_PIN;
    DBG_UART_PORT.OUTSET = DBG_UART_TX_PIN;
}

static inline void uart_delay_bit(void) {
    _delay_us(US_PER_S / DBG_UART_BAUD_RATE);
}

static void uart_write_byte(uint8_t byte) {
    DBG_UART_PORT.OUTCLR = DBG_UART_TX_PIN;
    uart_delay_bit();

    // transmit the byte LSB first over GPIO
    for (uint8_t i = 0; i < 8u; i++) {
        if (byte & 1u){ 
            DBG_UART_PORT.OUTSET = DBG_UART_TX_PIN;
        }
        else{             
            DBG_UART_PORT.OUTCLR = DBG_UART_TX_PIN;
        }
        uart_delay_bit();
        byte >>= 1;
    }

    DBG_UART_PORT.OUTSET = DBG_UART_TX_PIN;
    uart_delay_bit();
}

void io_debug_uart_write(const uint8_t *buf, uint16_t len) {
    if (!buf || len == 0u) return;
    for (uint16_t i = 0; i < len; i++) {
        uart_write_byte(buf[i]);
    }
}
