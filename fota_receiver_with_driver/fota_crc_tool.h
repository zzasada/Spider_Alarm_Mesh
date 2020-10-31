/*----------------------------------------------------------------------------
Copyright (c) 2020 LumenRadio AB
This code is the property of Lumenradio AB and may not be redistributed in any
form without prior written permission from LumenRadio AB.

This example is provided as is, without warranty.
----------------------------------------------------------------------------*/

#ifndef CRC_TOOL_H
#define CRC_TOOL_H

#include <stdint.h>

void fota_crc_init(
    uint32_t *state);

void fota_crc_update(
    uint32_t *state,
    const uint8_t *data,
    uint32_t length);

uint32_t fota_crc_get(
    uint32_t *state);

#endif
