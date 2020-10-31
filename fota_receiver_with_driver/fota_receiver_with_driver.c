/*----------------------------------------------------------------------------
Copyright (c) 2020 LumenRadio AB
This code is the property of Lumenradio AB and may not be redistributed in any
form without prior written permission from LumenRadio AB.

This example is provided as is, without warranty.
----------------------------------------------------------------------------*/

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include "../Common/common.h"

#include "fota_driver.h"

static const mira_net_config_t net_config = {
    .pan_id = PAN_ID_COMMON,
    .key = KEY_COMMON,
    .mode = MIRA_NET_MODE_MESH,
    .rate = MIRA_NET_RATE_FAST,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

/*
 * Identifies as a node.
 * Sends data to the root.
 */

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0), /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

PROCESS(main_proc, "Main process");

void mira_setup(void)
{
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    process_start(&main_proc, NULL);
}

const char *net_state(void)
{
    switch (mira_net_get_state()) {
    case MIRA_NET_STATE_NOT_ASSOCIATED:
        return "not associated";

    case MIRA_NET_STATE_IS_COORDINATOR:
        return "is coordinator";

    case MIRA_NET_STATE_ASSOCIATED:
        return "associated";

    case MIRA_NET_STATE_JOINED:
        return "joined";
    }
    return "unknown";
}

PROCESS_THREAD(main_proc, ev, data)
{
    static struct etimer timer;
    static uint8_t slot;
    static uint16_t seconds = 0;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("Version 2");

    mira_status_t result = mira_net_init(&net_config);
    if (result) {
        printf("FAILURE: mira_net_init returned %d\n", result);
        while (1);
    }

    /* Start fota process, which polls updates from network */
    fota_set_driver();
    if (mira_fota_init() != MIRA_SUCCESS) {
        printf("FOTA was already initialized");
    }

    while (1) {
        for (slot = 0; slot < NUMBER_OF_SLOTS; slot++) {
            if (mira_fota_is_valid(slot)) {
                printf("%s, Valid image for slot: %d with %ld bytes, version %d, time %d s\n", net_state(), slot, mira_fota_get_image_size(slot), mira_fota_get_version(slot), seconds);
            } else {
                printf("%s, No valid image available in cache for slot: %d, time: %d s\n",net_state(), slot, seconds);
            }
        }
        etimer_set(&timer, 5 * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        seconds = seconds +5;
    }

    PROCESS_END();
}
