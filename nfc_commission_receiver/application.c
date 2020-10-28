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


static void udp_listen_callback(
    mira_net_udp_connection_t *connection,
    const void *data,
    uint16_t data_len,
    const mira_net_udp_callback_metadata_t *metadata,
    void *storage)
{
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address),
        metadata->source_port);
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");

    printf("send back to: [%s]\n",
    mira_net_toolkit_format_address(buffer, metadata->source_address));
}

static void udp_motion_listen_callback(
    mira_net_udp_connection_t *connection,
    const void *data,
    uint16_t data_len,
    const mira_net_udp_callback_metadata_t *metadata,
    void *storage)
{
    led_state = !led_state;
    mira_gpio_set_value(LED2_PIN, led_state);
    printf("led_state_button:%d\n",led_state);
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("Received message from [%s]:%u: ",
        mira_net_toolkit_format_address(buffer, metadata->source_address),
        metadata->source_port);
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    printf("\n");
}


PROCESS(main_proc, "Main process");

PROCESS_THREAD(main_proc, ev, data) {
   
    mira_net_config_t netconf;
    PROCESS_BEGIN();

    printf("Main process started\n");

    memset(&netconf, 0, sizeof(mira_net_config_t));
    netconf.pan_id = app_config.net_panid;
    memcpy(netconf.key, app_config.net_key, 16);
    netconf.mode = MIRA_NET_MODE_ROOT;
    netconf.rate = app_config.net_rate;
    netconf.antenna = 0;

    mira_net_init(&netconf);
    mira_net_udp_listen(UDP_PORT, udp_listen_callback, NULL);
    mira_net_udp_listen(MOTION_UDP_PORT, udp_motion_listen_callback, NULL);

    PROCESS_END();
}
