//TODO: intialize SPI
//TODO: implement LIS3DH

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

#define LC709203F_I2CADDR_DEFAULT 0x0B     ///< LC709203F default i2c address
#define LC709203F_CMD_THERMISTORB 0x06     ///< Read/write thermistor B
#define LC709203F_CMD_INITRSOC 0x07        ///< Initialize RSOC calculation
#define LC709203F_CMD_CELLTEMPERATURE 0x08 ///< Read/write batt temperature
#define LC709203F_CMD_CELLVOLTAGE 0x09     ///< Read batt voltage
#define LC709203F_CMD_APA 0x0B             ///< Adjustment Pack Application
#define LC709203F_CMD_CELLITE 0x0F         ///< Read batt indicator to empty
#define LC709203F_CMD_ICVERSION 0x11       ///< Read IC version
#define LC709203F_CMD_ALARMRSOC 0x13       ///< Alarm on percent threshold
#define LC709203F_CMD_ALARMVOLT 0x14       ///< Alarm on voltage threshold
#define LC709203F_CMD_POWERMODE 0x15       ///< Sets sleep/power mode
#define LC709203F_CMD_STATUSBIT 0x16       ///< Temperature obtaining method

/*!  Battery temperature source */
typedef enum {
  LC709203F_TEMPERATURE_I2C = 0x0000,
  LC709203F_TEMPERATURE_THERMISTOR = 0x0001,
} lc709203_tempmode_t;

/*!  Chip power state */
typedef enum {
  LC709203F_POWER_OPERATE = 0x0001,
  LC709203F_POWER_SLEEP = 0x0002,
} lc709203_powermode_t;

/*!  Approx battery pack size */
typedef enum {
  LC709203F_APA_100MAH = 0x08,
  LC709203F_APA_200MAH = 0x0B,
  LC709203F_APA_500MAH = 0x10,
  LC709203F_APA_1000MAH = 0x19,
  LC709203F_APA_2000MAH = 0x2D,
  LC709203F_APA_3000MAH = 0x36,
} lc709203_adjustment_t;

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

PROCESS(lc709203f, "LC709203F process");

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    process_start(&lc709203f, NULL);
}

static void init_i2c(mira_i2c_context_t *context){
    mira_status_t ret = mira_i2c_init(context, I2C_SCL, I2C_SDA, MIRA_I2C_FREQUENCY_100_KHZ);
    if(ret == MIRA_SUCCESS){
        printf("AK9756: I2C successfully intialized.\n");
    }else if(ret == MIRA_ERROR_UNKNOWN){
        printf("AK9756: I2C unknown error\n");
    }else if(ret == MIRA_I2C_ERROR_INVALID_PIN){
        printf("AK9756: I2C invalid pin.\n");
    }else if(ret == MIRA_ERROR_NOT_IMPLEMENTED){
        printf("AK9756: I2C not implemented???\n");
    }else{
        printf("AK9756: I2C shouldn't see this error.\n");
    }
}

static mira_status_t read_register(mira_i2c_context_t *context, uint8_t register_address, uint8_t *byte_array, uint8_t bytes_to_read){
        mira_i2c_set_transfer_type_read(context, &register_address, bytes_to_read, byte_array, bytes_to_read);
        mira_i2c_set_slave_address(context, LC709203F_I2CADDR_DEFAULT);
        return mira_i2c_transfer_start(context);
}

// static mira_status_t write_register(mira_i2c_context_t *context, uint8_t register_address, uint8_t command){
//         uint8_t dst_local[2];
//         dst_local[0] = register_address;
//         dst_local[1] = command;
//         mira_i2c_set_transfer_type_write(context, &dst_local[0], 2);
//         mira_i2c_set_slave_address(context, LC709203F_I2CADDR_DEFAULT);
//         return mira_i2c_transfer_start(context);
// }

// void printDouble(double v, int decimalDigits)
// {
//   int i = 1;
//   int intPart, fractPart;
//   for (;decimalDigits!=0; i*=10, decimalDigits--);
//   intPart = (int)v;
//   fractPart = (int)((v-(double)(int)v)*i);
//   if(fractPart < 0) fractPart *= -1;
//   printf("%i.%i", intPart, fractPart);
// }


PROCESS_THREAD(lc709203f, ev, data){
    static struct etimer timer;
    static mira_status_t ret;
    static mira_i2c_context_t context;
    static uint8_t byte_array[2];

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("Starting Test LC709203F.\n");

    init_i2c(&context);

        //Confirm we can communicate with AK9756
    ret = read_register(&context, LC709203F_CMD_ICVERSION, &byte_array[0], 1);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        // printf("AK9756: I2C transfer successful\n");
        if(byte_array[0] == LC709203F_CMD_ICVERSION){
            printf("LC709203F: success in getting manufacturer ID:%02X\n",byte_array[0]);
        }else{
            printf("LC709203F: invalid manufacturer ID:%02X\n",byte_array[0]);
        }
    } else {
        printf("LC709203F: Error with I2C transfer.\n");
    }


    while (1) {
        ret = read_register(&context, LC709203F_CMD_CELLVOLTAGE, &byte_array[0], 2);
        if(ret == MIRA_SUCCESS) {
            PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
            printf("LC709203F: Battery Voltage:%02X %02X\n",byte_array[0], byte_array[1]);
        } else {
            printf("LC709203F: Error with I2C transfer.\n");
        }

        etimer_set(&timer, 1 * CLOCK_SECOND);
        PROCESS_WAIT_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}
