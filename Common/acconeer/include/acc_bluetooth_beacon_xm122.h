// Copyright (c) Acconeer AB, 2019
// All rights reserved

#ifndef ACC_BLUETOOTH_BEACON_XM122_H_
#define ACC_BLUETOOTH_BEACON_XM122_H_

#include <stdbool.h>
#include <stdint.h>


/**@brief Function for initializing BLE stack
 */
void acc_bluetooth_beacon_init(void);


/**@brief Function for starting and updating data in Bluetooth advertising.
 *
 * @param[in] interval_ms  The advertising interval in ms
 * @param[in] data1        First two bytes of advertising data
 * @param[in] data2        Last two bytes of advertising data
 *
 * @return True if successful.
 */
bool acc_bluetooth_beacon_update(uint32_t interval_ms, uint16_t *data1, uint16_t *data2);


/**@brief Function for stopping Bluetooth advertising.
 *
 * @return True if successful.
 */
bool acc_bluetooth_beacon_stop(void);


#endif
