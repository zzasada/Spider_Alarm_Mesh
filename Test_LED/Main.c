//TODO: intialize SPI
//TODO: implement LIS3DH

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "pwm/pwm_lib.h"

#define HEARTBEAT_INTERVAL  60
#define SEND_INTERVAL       60
#define CHECK_NET_INTERVAL  1
#define ALARM_INTERVAL      1
#define SOLENOID_INTERVAL   500 //ms

#define IN1     MIRA_GPIO_PIN(1,11)
#define IN2     MIRA_GPIO_PIN(1,12)

#define LIS3DL_INT_1    MIRA_GPIO_PIN(1,06)
#define LIS3DL_INT_2    MIRA_GPIO_PIN(0,28)

bool alarm_on = false;
bool connected = false;

#define DEBUG_MSG_MAX_SIZE 32
char debug_msg[DEBUG_MSG_MAX_SIZE];
int debug_index = 0;
bool debug_message_flag = false;

long tick_counter = 0;

enum device_type my_device_type = LATCH;
enum message_type my_message_type = -1;

bool solenoid_command_flag = false;
int solenoid_command = -1;

static const mira_net_config_t net_config = {
    .pan_id = PAN_ID_COMMON,
    .key = KEY_COMMON,
    .mode = MIRA_NET_MODE_MESH,
    .rate = MIRA_NET_RATE_SLOW,
    .antenna = 0,
    .prefix = NULL /* default prefix */
};

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

void solenoid_out(void){
    printf("solenoid_out() 11=50 12=0 start\n");
    pwm_set_value(80);
    mira_gpio_set_value(IN2, 0);
}

void solenoid_in(void){
    printf("solenoid_in() 11=0 12=1 \n");
    pwm_set_value(0);
    mira_gpio_set_value(IN2, 1);
}

void solenoid_off(void){
    printf("solenoid_off()\n");
    pwm_set_value(0);
    mira_gpio_set_value(IN2, 0);
}

char udp_messages[32];
uint8_t next_udp_message_index = 0;

static void udp_listen_callback( mira_net_udp_connection_t *connection, const void *data, uint16_t data_len, const mira_net_udp_callback_metadata_t *metadata, void *storage){
    char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    uint16_t i;

    printf("General message from [%s]: port=%u: data_length=%i ", mira_net_toolkit_format_address(buffer, metadata->source_address),metadata->source_port, data_len);
    for (i = 0; i < data_len - 1; i++) {
        printf("%c", ((char *) data)[i]);
    }
    char command = ((char *)data)[0];

    if(command == '1'){
        solenoid_command_flag = true;
        solenoid_command = 1;
    }else if(command == '2'){
        solenoid_command_flag = true;
        solenoid_command = 2;
    }
    printf("\n");
}

PROCESS(solenoid_proc, "Solenoid process");
PROCESS(main_proc, "Main process");
PROCESS(rtt_proc, "RTT process");
PROCESS(udp_proc, "UDP process");

void pin_setup(void){
    pwm_set_value(0);
    pwm_enable();
    mira_gpio_set_dir(IN2, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(IN2, 0);
}

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    pin_setup();

    process_start(&main_proc, NULL);
    process_start(&rtt_proc, NULL);
    process_start(&udp_proc, NULL);
    process_start(&solenoid_proc, NULL);
}

PROCESS_THREAD(solenoid_proc, ev, data) {
    static struct etimer timer;
    // etimer_set(&timer, SOLENOID_INTERVAL * CLOCK_SECOND);

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    for (;;) {
        /* Yield to give other processes access to the CPU */
        PROCESS_PAUSE();
        if(solenoid_command_flag){
            if(solenoid_command == 1){
                solenoid_in();
                etimer_set(&timer, SOLENOID_INTERVAL);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
                solenoid_off();
            }else if(solenoid_command == 2){
                solenoid_out();
                etimer_set(&timer, SOLENOID_INTERVAL);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
                solenoid_off();
            }
            solenoid_command_flag = false;
            solenoid_command = 0;
        }
        etimer_set(&timer, 1000);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}

PROCESS_THREAD(udp_proc, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    for (;;) {
        PROCESS_PAUSE();
        if(next_udp_message_index > 0){
            if(udp_messages[next_udp_message_index - 1] == '0'){
                printf("Solenoid out from udp received.");    
                solenoid_out();
                etimer_set(&timer, SOLENOID_INTERVAL);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
                solenoid_off();
            }else if(udp_messages[next_udp_message_index - 1] == '1'){
                printf("Solenoid in from udp received");
                solenoid_in();
                etimer_set(&timer, SOLENOID_INTERVAL);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
                solenoid_off();
            }else{
                printf("Unknown udp message. %c", udp_messages[next_udp_message_index -1]);
            }
            next_udp_message_index--;
        }
    }
    PROCESS_END()
}

PROCESS_THREAD(rtt_proc, ev, data) {
    uint8_t buffer[16];
    int len;
    static struct etimer timer;
    // etimer_set(&timer, SOLENOID_INTERVAL * CLOCK_SECOND);

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    for (;;) {
        /* Yield to give other processes access to the CPU */
        PROCESS_PAUSE();
        do {
            len = mira_rtt_read(-1, 0, buffer, sizeof(buffer));
            if (len > 0) {
                printf("char_in=%c, length=%i\n",(int)buffer[0], len);
                if(buffer[0] == '1'){
                    solenoid_in();
                    etimer_set(&timer, SOLENOID_INTERVAL);
                    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
                    solenoid_off();
                }else if(buffer[0] == '2'){
                    solenoid_out();
                    etimer_set(&timer, SOLENOID_INTERVAL);
                    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
                    solenoid_off();
                }else if(buffer[0] == '3'){
                    solenoid_off();
                }else if(buffer[0] == '4'){
                    printf("option 4 selected.");
                }else{
                    printf("Invalid command, Commands are 1=start_alarm, 2=stop_alarm, 3=solenoid_out, 4=solenoid_in\n");
                }
            }
        } while (len > 0);
    }
    PROCESS_END()
}

PROCESS_THREAD(main_proc, ev, data)
{
    static struct etimer timer;

    static mira_net_udp_connection_t *udp_connection;

    static mira_net_address_t net_address;
    // static char buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];
    static mira_status_t res;
    char message[32] = "-1,-1";

    static mira_net_address_t parent_address;
    // static char parent_buffer[MIRA_NET_MAX_ADDRESS_STR_LEN];

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("Starting Node (Sender).\n");
    printf("Sending one packet every %d seconds\n", SEND_INTERVAL);

    mira_net_init(&net_config);

    pwm_init();
    udp_connection = mira_net_udp_connect(NULL, 0, udp_listen_callback, NULL);
    mira_net_udp_listen(UDP_PORT, udp_listen_callback, NULL);
    mira_net_state_t old_net_state = MIRA_NET_STATE_JOINED;

    while (1) {
        mira_net_state_t net_state = mira_net_get_state();

        if (net_state != MIRA_NET_STATE_JOINED) {
            if(old_net_state != net_state){
                printf(
                    "Waiting for network (state is %s) tick=%lu\n",
                    net_state == MIRA_NET_STATE_NOT_ASSOCIATED ? "not associated"
                    : net_state == MIRA_NET_STATE_ASSOCIATED ? "associated"
                    : net_state == MIRA_NET_STATE_JOINED ? "joined"
                    : "UNKNOWN", tick_counter
                );
                tick_counter++;
                old_net_state = net_state;
            }
            etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
        } else {
            res = mira_net_get_root_address(&net_address);
            res = mira_net_get_parent_address(&parent_address);

            uint32_t link_metric = mira_net_get_parent_link_metric();
            uint32_t etx = (link_metric * 125) / 16;

            // device_id, device_type, link_strength


            if (res != MIRA_SUCCESS) {
                printf("Waiting for root address (res: %d)\n", res);
                etimer_set(&timer, CHECK_NET_INTERVAL * CLOCK_SECOND);
            } else {
                // printf("Sending message to root: %s, parent: %s , ETX: %lu /1000 \n",
                // mira_net_toolkit_format_address(buffer, &net_address),
                // mira_net_toolkit_format_address(parent_buffer, &parent_address), 
                // etx);

                snprintf(message, 32, "%i,%lu", my_device_type, etx);
                printf("Sent to Root, device_type=%i, ETX: %lu /1000 message=%s\n", my_device_type, etx, message);
                mira_net_udp_send_to(udp_connection, &net_address, UDP_PORT, message, strlen(message));
                etimer_set(&timer, SEND_INTERVAL * CLOCK_SECOND);
            }
        }

        if(debug_message_flag){
            debug_message_flag = false;
            printf("received {%s} from debug.", debug_msg);
            debug_index = 0;
        }
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    mira_net_udp_close(udp_connection);

    PROCESS_END();
}
