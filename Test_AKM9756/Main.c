//TODO: intialize SPI
//TODO: implement LIS3DH

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

#define AKM9756_INTERVAL 1
#define AK9756_ADDRESS 0x64

#define AK9756_WIA1 0x00
#define AK9756_WIA2 0x01

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

PROCESS(akm9756, "AKM9756 process");

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    process_start(&akm9756, NULL);
}


PROCESS_THREAD(akm9756, ev, data){
    static struct etimer timer;
    // static uint8_t tx_buffer[2];
    // static uint8_t rx_buffer[sizeof(tx_buffer)];
    mira_status_t ret;

    static mira_i2c_context_t context;
    uint8_t device_address = AK9756_ADDRESS;
    uint8_t register_address = AK9756_WIA1;
    uint16_t register_address_size = 1;
    static uint8_t byte_array[2];
    uint16_t byte_array_size = 2;

    static uint8_t  dst[2];
    uint16_t        nb_dst_bytes = 2;
    static uint8_t  command_buffer[1];
    uint16_t        nb_command_bytes = 1;
    // static uint8_t  write_data_buffer[1];
    // uint16_t        nb_write_data_bytes = 1;

    command_buffer[0] = 0x00;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("Starting Test AKM9756.\n");

    ret = mira_i2c_init(&context, I2C_SCL, I2C_SDA, MIRA_I2C_FREQUENCY_100_KHZ);
    if(ret == MIRA_SUCCESS){
        printf("I2C successfully intialized.\n");
    }else if(ret == MIRA_ERROR_UNKNOWN){
        printf("I2C unknown error\n");
    }else if(ret == MIRA_I2C_ERROR_INVALID_PIN){
        printf("I2C invalid pin.\n");
    }else if(ret == MIRA_ERROR_NOT_IMPLEMENTED){
        printf("I2C not implemented???\n");
    }else{
        printf("I2C shouldn't see this error.\n");
    }


    while (1) {
        byte_array[0] = 0;
        byte_array[1] = 0;
        mira_i2c_construct_write_buffer(dst, &nb_dst_bytes, command_buffer, nb_command_bytes, NULL, 0);
        mira_i2c_set_transfer_type_write(&context, dst, nb_dst_bytes);
        mira_i2c_set_transfer_type_read(&context, &register_address, register_address_size, &byte_array[0], byte_array_size);
        mira_i2c_set_slave_address(&context, device_address);
        ret = mira_i2c_transfer_start(&context);
        if(ret == MIRA_SUCCESS) {
            PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
            printf("I2C transfer successful:%02X,%02X \n",byte_array[0], byte_array[1]);
        } else {
            printf("Error with I2C transfer.\n");
        }

        etimer_set(&timer, 1 * CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}
