/*----------------------------------------------------------------------------
Copyright (c) 2020 LumenRadio AB
This code is the property of Lumenradio AB and may not be redistributed in any
form without prior written permission from LumenRadio AB.

This example is provided as is, without warranty.
----------------------------------------------------------------------------*/

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

#define UDP_PORT 456
#define SEND_INTERVAL 10
#define CHECK_NET_INTERVAL 1

static mira_net_udp_connection_t *response_connection;
static char response_data[256];
static uint16_t response_data_len = 0;
// static mira_net_address_t response_address;

/*
 * Identifies as a node.
 * Sends data to the root.
 */
static const mira_net_config_t net_config = {
    .pan_id = PAN_ID_COMMON,
    .key = KEY_COMMON,
    .mode = MIRA_NET_MODE_MESH,
    .rate = MIRA_NET_RATE_MID,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

PROCESS(main_proc, "Main process");
PROCESS(response_proc, "MAIN process");

static void udp_listen_callback(mira_net_udp_connection_t *connection, const void *data, uint16_t data_len, const mira_net_udp_callback_metadata_t *metadata, void *storage){
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address),metadata->source_port);
    for (i = 0; i < data_len; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");

    response_connection = connection;
    memcpy(response_data, data, data_len);
    response_data_len = data_len;

    process_poll(&response_proc);
}

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    process_start(&main_proc, NULL);
    process_start(&response_proc, NULL);
}

PROCESS_THREAD(response_proc, ev, data){
    // static mira_net_udp_connection_t *response_udp_connection;
    static struct etimer response_timer;
    static mira_net_address_t response_net_address;
    static char response_buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    static mira_status_t response_res;
    // static const char *response_message = "id:100B,battery:True,fw:1";

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();
    while (1) {
        /* Sleep until next event, regardless of reason */
        PROCESS_YIELD();
        if(response_data_len > 0){
            printf("Request to udp respond\n");
            response_res = mira_net_get_root_address(&response_net_address);
            
            if (response_res != MIRA_SUCCESS) {
                    printf("Waiting for root address (res: %d)\n", response_res);
                    etimer_set(&response_timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
            } else {
                printf("Sending to address: %s,%s\n", mira_net_toolkit_format_address(response_buffer, &response_net_address), response_data);
                mira_net_udp_send_to(response_connection, &response_net_address, UDP_PORT, response_data, strlen(response_data));
                mira_net_udp_close(response_connection);
                response_data_len = 0;
            }
        }
    }
    PROCESS_END();
}

PROCESS_THREAD(main_proc, ev, data)
{
    static struct etimer timer;

    static mira_net_udp_connection_t *udp_connection;

    static mira_net_address_t net_address;
    static char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    static mira_status_t res;
    static const char *message = "hb,id:100B,battery:True,fw:1";

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("Starting Node (Sender).\n");
    printf("Sending one packet every %d seconds\n", SEND_INTERVAL);

    mira_status_t result = mira_net_init(&net_config);
    if (result) {
        printf("FAILURE: mira_net_init returned %d\n", result);
        while (1);
    }

    /*
     * Open a connection, but don't specify target address yet, which means
     * only mira_net_udp_send_to() can be used to send packets later.
     */
    udp_connection = mira_net_udp_connect(NULL, 0, udp_listen_callback, NULL);

    while (1) {
        mira_net_state_t net_state = mira_net_get_state();

        if (net_state != MIRA_NET_STATE_JOINED) {
            printf(
                "Waiting for network (state is %s)\n",
                net_state == MIRA_NET_STATE_NOT_ASSOCIATED ? "not associated"
                : net_state == MIRA_NET_STATE_ASSOCIATED ? "associated"
                : net_state == MIRA_NET_STATE_JOINED ? "joined"
                : "UNKNOWN"
            );
            etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
        } else {
            /* Try to retrieve the root address. */
            res = mira_net_get_root_address(&net_address);

            if (res != MIRA_SUCCESS) {
                printf("Waiting for root address (res: %d)\n", res);
                etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
            } else {
                /*
                 * Root address is successfully retrieved, send a message to the
                 * root node on the given UDP Port.
                 */
                printf("Sending to address: %s %s\n", mira_net_toolkit_format_address(buffer, &net_address), message);
                mira_net_udp_send_to(udp_connection, &net_address, UDP_PORT, message, strlen(message));
                etimer_set(&timer, SEND_INTERVAL * CLOCK_SECOND);
            }
        }
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    mira_net_udp_close(udp_connection);

    PROCESS_END();
}
