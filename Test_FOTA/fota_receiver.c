/*----------------------------------------------------------------------------
Copyright (c) 2020 LumenRadio AB
This code is the property of Lumenradio AB and may not be redistributed in any
form without prior written permission from LumenRadio AB.

This example is provided as is, without warranty.
----------------------------------------------------------------------------*/

#include <mira.h>
#include "fota_update.h"

#include <stdio.h>
#include <string.h>


#define NEW_VERSION

static const mira_net_config_t net_config = {
    .pan_id = 0x8724a685,
    .key = {0x93, 0x30, 0x0c, 0x4b, 0x3a, 0xd1, 0x0f, 0x8b, 0x69, 0xc3, 0xa1, 0x17, 0x98, 0x5f, 0xc0, 0x47},
    .mode = MIRA_NET_MODE_MESH,
    .rate = MIRA_NET_RATE_FAST,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

/*         aa55aaffaa55aaffaa55aaffaa55aaff */

/*
 * Identifies as a node.
 * Sends data to the root.
 */

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_UART(0), /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

PROCESS(main_proc, "Main process");

void mira_setup(void){
    mira_status_t uart_ret;
    mira_uart_config_t uart_config = {
        .baudrate = 115200,
        .tx_pin = MIRA_GPIO_PIN(0, 6),
        .rx_pin = MIRA_GPIO_PIN(0, 8)
    };

    MIRA_MEM_SET_BUFFER(12288);

    uart_ret = mira_uart_init(0, &uart_config);
    if (uart_ret != MIRA_SUCCESS) {
        /* Nowhere to send an error message */
    }

    process_start(&main_proc, NULL);
}


PROCESS_THREAD(main_proc, ev, data)
{
    // static struct etimer timer;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    mira_status_t result = mira_net_init(&net_config);
    if (result) {
        printf("FAILURE: mira_net_init returned %d\n", result);
        while (1);
    }
    #ifdef NEW_VERSION
    printf("THIS IS A NEW VERSION 2!\n");
    #endif

    /* Start fota process, which polls updates from network */
    fota_init();

    PROCESS_END();
}
