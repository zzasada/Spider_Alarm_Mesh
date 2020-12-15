#include "crc.h"

#include <stdio.h>

void crc_init(
    crc_t *crc)
{
    crc->state = 0xFFFFFFFF;
    crc->poly = 0xEDB88320;
}

void crc_update(
    crc_t *crc,
    const uint8_t *data,
    uint32_t length)
{
    register uint32_t byte, poly, state, mask;
    register int i;

    state = crc->state;
    poly = crc->poly;
    while (length--) {
        byte = *(data++);
        state ^= byte;
        for (i = 8; i; i--) {
            mask = -(state & 1);
            state = (state >> 1) ^ (poly & mask);
        }
    }
    crc->state = state;
}

uint32_t crc_get(
    const crc_t *crc)
{
    return ~crc->state;
}
