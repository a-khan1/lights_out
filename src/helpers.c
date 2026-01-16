#include "helpers.h"

uint8_t bit_index(uint8_t r, uint8_t c) {
    return (uint8_t)(r * NUM_COLS + c); 
}
