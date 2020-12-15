#ifndef CRC_H
#define CRC_H

#include <stdint.h>

typedef struct {
    uint32_t state;
    uint32_t poly;
} crc_t;

void crc_init(
    crc_t *crc);

void crc_update(
    crc_t *crc,
    const uint8_t *data,
    uint32_t length);

uint32_t crc_get(
    const crc_t *crc);

#endif
