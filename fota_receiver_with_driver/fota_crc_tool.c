/*----------------------------------------------------------------------------
Copyright (c) 2020 LumenRadio AB
This code is the property of Lumenradio AB and may not be redistributed in any
form without prior written permission from LumenRadio AB.

This example is provided as is, without warranty.
----------------------------------------------------------------------------*/

#include "fota_crc_tool.h"

void fota_crc_init(uint32_t *state)
{
    *state = 0xFFFFFFFF;
}

void fota_crc_update(uint32_t *state, const uint8_t *data, uint32_t length)
{
    int i;
    while (length--) {
        *state ^= *(data++);
        for (i = 0; i < 8; i++) {
            *state = (*state >> 1) ^ ((*state & 1) ? 0xEDB88320 : 0);
        }
    }
}

uint32_t fota_crc_get(uint32_t *state)
{
    return ~*state;
}
