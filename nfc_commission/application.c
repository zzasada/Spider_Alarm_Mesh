/*
 * Copyright (c) 2018, LumenRadio AB All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <mira.h>
#include <stdio.h>
#include <string.h>

#include "application.h"

#include "app-config.h"

#define UDP_PORT 456
#define CHECK_NET_INTERVAL 5
#define MOTION_UDP_PORT 7000

static uint8_t led_state;
#define LED2_PIN MIRA_GPIO_PIN(0,14)
#define VIBRA_INT_PIN MIRA_GPIO_PIN(0,27)


static void udp_listen_callback(mira_net_udp_connection_t *connection, const void *data, uint16_t data_len, const mira_net_udp_callback_metadata_t *metadata, void *storage)
{
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address), metadata->source_port);
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");

    printf("send back to: [%s]\n",
    mira_net_toolkit_format_address(buffer, metadata->source_address));
}

static void udp_motion_listen_callback(mira_net_udp_connection_t *connection, const void *data, uint16_t data_len, const mira_net_udp_callback_metadata_t *metadata, void *storage)
{
    led_state = !led_state;
    mira_gpio_set_value(LED2_PIN, led_state);
    printf("led_state_button:%d\n",led_state);
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address), metadata->source_port);
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");
}


PROCESS(main_proc, "Main process");
PROCESS(vibra_proc, "Vibra process");

PROCESS_THREAD(vibra_proc, ev , data)
{
    static mira_net_udp_connection_t *udp_connection;
    static mira_net_address_t root_address;
    static char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    // static mira_status_t res;
    static const char *message = "Motion Pin Triggered";

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    udp_connection = mira_net_udp_connect(NULL, 0, udp_motion_listen_callback, NULL);
    mira_net_udp_listen(MOTION_UDP_PORT, udp_motion_listen_callback, NULL);

    while(1){
      mira_net_get_root_address(&root_address);
      PROCESS_WAIT_UNTIL(mira_gpio_has_triggered_edge_event(VIBRA_INT_PIN));
      printf("Motion pin high\n");
      printf("Sending to address: %s\n",
      mira_net_toolkit_format_address(buffer, &root_address));
      mira_net_udp_send_to(udp_connection, &root_address, MOTION_UDP_PORT, message, strlen(message));
    }

    PROCESS_END();
}

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
    PROCESS_BEGIN();

    printf("Main process started\n");

    memset(&netconf, 0, sizeof(mira_net_config_t));
    netconf.pan_id = app_config.net_panid;
    memcpy(netconf.key, app_config.net_key, 16);
    netconf.mode = MIRA_NET_MODE_MESH;
    netconf.rate = app_config.net_rate;
    netconf.antenna = 0;

    mira_net_init(&netconf);

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
            res = mira_net_get_parent_address(&parent_address);

            uint32_t link_metric = mira_net_get_parent_link_metric();
            uint32_t etx = (link_metric * 125) / 16;

            if (res != MIRA_SUCCESS) {
                printf("Waiting for root address (res: %d)\n", res);
                etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
            } else {
                /*
                 * Root address is successfully retrieved, send a message to the
                 * root node on the given UDP Port.
                 */
                printf("Sending message to root: %s, parent: %s , ETX: %lu /1000 \n",
                mira_net_toolkit_format_address(buffer, &net_address),
                mira_net_toolkit_format_address(parent_buffer, &parent_address), etx);
                mira_net_udp_send_to(udp_connection, &net_address, UDP_PORT, message, strlen(message));
                etimer_set(&timer, app_config.update_interval * CLOCK_SECOND);
            }
        }
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }
    mira_net_udp_close(udp_connection);
    //net_status_init();

    //sensors_init();

    PROCESS_END();
}
