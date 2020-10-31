//TODO: turn net rate down to MID
//TODO: allow PAN_ID, KEY, and net rate to be written by nfc
#include <mira.h>
#include <stdio.h>
#include <string.h>
#include "application.h"
#include "config/app-config.h"
#include "../Common/common.h"

#define CHECK_NET_INTERVAL 5

static void udp_listen_callback(mira_net_udp_connection_t *connection, const void *data, uint16_t data_len, const mira_net_udp_callback_metadata_t *metadata, void *storage)
{
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("APP: Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address), metadata->source_port);
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");

    printf("APP: send back to: [%s]\n",
    mira_net_toolkit_format_address(buffer, metadata->source_address));
}

const char *net_status(void){
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


PROCESS(main_proc, "Main process");

PROCESS_THREAD(main_proc, ev, data) {
   

    static struct etimer timer;

    static mira_net_udp_connection_t *udp_connection;

    static mira_net_address_t net_address;
    static char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    static mira_net_address_t parent_address;
    static char parent_buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    static mira_status_t res;
    static const char *message = "Hello Network";
    mira_net_config_t netconf;
    static uint32_t seconds;

    PROCESS_BEGIN();

    printf("APP: Main process started\n");

    memset(&netconf, 0, sizeof(mira_net_config_t));
    // netconf.pan_id = app_config.net_panid;
    netconf.pan_id = PAN_ID_COMMON;
    // memcpy(netconf.key, app_config.net_key, KEY_LENGTH);
    memcpy(netconf.key, KEY_COMMON, KEY_LENGTH);      //////////////// TODO: figure out why I cant get this from common.h

    printf("APP: Net key:");
    int i;
    for (i = 0; i < KEY_LENGTH; i++){
        printf(" %02x", netconf.key[i]);
    }
    printf("\n");

    netconf.mode = MIRA_NET_MODE_MESH;
    // netconf.rate = app_config.net_rate;
    netconf.rate = MIRA_NET_RATE_FAST;
    netconf.antenna = 0;

    mira_net_init(&netconf);

    udp_connection = mira_net_udp_connect(NULL, 0, udp_listen_callback, NULL);
    while (1) {
        mira_net_state_t net_state = mira_net_get_state();

        if (net_state != MIRA_NET_STATE_JOINED) {
            printf("APP: %s, Waiting for network connection %lu s\n", net_status(), seconds);
            etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
        } else {
            /* Try to retrieve the root address. */
            res = mira_net_get_root_address(&net_address);
            res = mira_net_get_parent_address(&parent_address);

            uint32_t link_metric = mira_net_get_parent_link_metric();
            uint32_t etx = (link_metric * 125) / 16;

            if (res != MIRA_SUCCESS) {
                printf("APP: Waiting for root address (res: %d)\n", res);
                etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
            } else {
                printf("APP: Sending message to root: %s, parent: %s , ETX: %lu /1000 \n",
                mira_net_toolkit_format_address(buffer, &net_address),
                mira_net_toolkit_format_address(parent_buffer, &parent_address), etx);
                mira_net_udp_send_to(udp_connection, &net_address, UDP_PORT, message, strlen(message));
                etimer_set(&timer, app_config.update_interval * CLOCK_SECOND);
            }
        }
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        seconds = seconds + CHECK_NET_INTERVAL;
    }
    mira_net_udp_close(udp_connection);

    PROCESS_END();
}
