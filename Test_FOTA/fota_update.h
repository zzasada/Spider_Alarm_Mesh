
#ifndef FOTA_UPDATE_H
#define FOTA_UPDATE_H

#include <stdint.h>
#include <stdbool.h>

#define FOTA_FW_SLOT_ID 0
#define FOTA_INVALID_CRC 0xffffffff

/**
 * @brief Initialize AirGlow FOTA
 *
 * @param version Version of running firmware
 *
 * @return Status of the operation
 * @retval 0 on success, -1 on failure
 */
int fota_init(
    void);

/**
 * @brief Get CRC of running application
 *
 * @return CRC of the application
 */
uint32_t fota_get_app_crc(
    void);

/**
 * @brief Get size of running application
 *
 * @return CRC of the application
 */
uint32_t fota_get_app_size(
    void);

/**
 * @brief Get CRC of FOTA buffer
 *
 * @return CRC of the FOTA buffer, 0xffffffff if contents are invalid
 */
uint32_t fota_get_fota_crc(
    void);

/**
 * @brief Schedule a firmware upgrade with contents of the FOTA buffer
 *
 * @param new_crc     Expected CRC of FOTA buffer
 * @param force_reset Force the node to reset at the scheduled time
 */
void fota_ag_perform_upgrade(
    uint32_t new_crc,
    bool force_reset);

#endif