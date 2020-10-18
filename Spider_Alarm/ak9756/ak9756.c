#include "ak9756.h"
#include "stdbool.h"
#include <mira.h>
#include "common.h"
#include "ak9756_reg.h"

PROCESS(ak9756_setup_proc, "AK9756 Setup Process");
PROCESS(ak9756_proc, "AK9756 process");

void alert_setup(void){
    printf("AK9756: Alert setup.\n");

    mira_status_t set_dir = mira_gpio_set_dir(ALERT, MIRA_GPIO_DIR_IN);
    if(set_dir == MIRA_SUCCESS ){
        printf("AK9756: successfully enabled pin dir.\n");
    }else{
        printf("AK9756: failed to enable pin dir.\n");
    }

    mira_status_t disable_edge = mira_gpio_disable_edge_event(ALERT);
    if(disable_edge == MIRA_SUCCESS ){
        printf("AK9756: successfully disabled edge detection.\n");
    }else{
        printf("AK9756: failed to disable edge detection.\n");
    }

    mira_status_t enable_edge = mira_gpio_enable_edge_event(ALERT, MIRA_EDGE_FALLING);
    if(enable_edge == MIRA_SUCCESS ){
        printf("AK9756: successfully enabled alarm.\n");
    }else{
        printf("AK9756: failed to enabled alarm.\n");
    }
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

static mira_status_t read_register(mira_i2c_context_t *context, uint8_t register_address, uint8_t *byte_array){
        mira_i2c_set_transfer_type_read(context, &register_address, 1, byte_array, 1);
        mira_i2c_set_slave_address(context, AK9756_SLAVEADDR);
        return mira_i2c_transfer_start(context);
}

static mira_status_t write_register(mira_i2c_context_t *context, uint8_t register_address, uint8_t command){
        // dst[0] = register_address;
        // dst[1] = command;
        // mira_i2c_set_transfer_type_write(context, dst, nb_dst_bytes);
        // mira_i2c_set_slave_address(context, AK9756_SLAVEADDR);
        // return mira_i2c_transfer_start(context);
        uint8_t dst_local[2];
        dst_local[0] = register_address;
        dst_local[1] = command;
        mira_i2c_set_transfer_type_write(context, &dst_local[0], 2);
        mira_i2c_set_slave_address(context, AK9756_SLAVEADDR);
        return mira_i2c_transfer_start(context);
}

PROCESS_THREAD(ak9756_proc, ev , data){

    // static struct etimer timer;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("AK9756 main thread begin.\n");

    alert_setup();

    while (1) {
        PROCESS_WAIT_UNTIL(mira_gpio_has_triggered_edge_event(ALERT));
        printf("AK9756 motion detected!!!!!!!!! Need to pull\n");
        //TODO: notify the network sender motion has been detected.
    }


    PROCESS_END();
}

PROCESS_THREAD(ak9756_setup_proc, ev, data){
    static struct etimer timer;
    static mira_i2c_context_t context;
    static uint8_t byte_array[2];
    static mira_status_t ret;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("AK9756: Initializing AKM9756.\n");

    init_i2c(&context);

    //Confirm we can communicate with AK9756
    ret = read_register(&context, AK9756_REG_ADDR_WIA1, &byte_array[0]);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        // printf("AK9756: I2C transfer successful\n");
        if(byte_array[0] == AK9756_REG_VALUE_WIA1){
            printf("AK9756: success in getting manufacturer ID:%02X\n",byte_array[0]);
        }else{
            printf("AK9756: invalid manufacturer ID:%02X\n",byte_array[0]);
        }
    } else {
        printf("AK9756: Error with I2C transfer.\n");
    }

    //soft reset (0x13, 0xFF)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL1, CNTL1_SOFT_RESET);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: soft reset successful \n");

        etimer_set(&timer, 100); //Wait 1ms for soft reset
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    } else {
        printf("AK9756: soft reset failed \n");
    }

    // 100% Gain, CM2 (0x14, 0xBA)
    // write_register(&context, &dst[0], nb_dst_bytes, AK9756_REG_ADDR_CNTL2, CNTL2_DEMO2);
    ret = write_register(&context, AK9756_REG_ADDR_CNTL2, CNTL2_DEMO2);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL2 successful \n");
    } else {
        printf("AK9756: CNTL2 failed \n");
    }
    
    // // Set up the CNTL3 for ODR=10Hz and TMP & IR Fc = ODR/6 (0x15, 0xAA)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL3, CNTL3_DEMO_10HZ);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL3 successful \n");
    } else {
        printf("AK9756: CNTL3 failed \n");
    }
    
    //TODO: confirm ****
    // // Set up the CNTL4 register for Demo mode HD01E, HD10E, HDIE (0x16, 0xF0)
    printf("AK9756: writing register:%02X, command:%02X\n",AK9756_REG_ADDR_CNTL4, CNTL4_HPD_DEMO);
    ret = write_register(&context, AK9756_REG_ADDR_CNTL4, CNTL4_HPD_DEMO );
    ret = write_register(&context, AK9756_REG_ADDR_CNTL4, 0xF0 );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL4 successful \n");
    } else {
        printf("AK9756: CNTL4 failed \n");
    }

    // // Configure CNTL5 for Bypass Mode
    // // Write to the CNTL5 register to configure the FIFO (0x17, 0xC0)
    printf("AK9756: writing register:%02X, command:%02X\n",AK9756_REG_ADDR_CNTL5, CNTL5_HPD_DEMO);
    ret = write_register(&context, AK9756_REG_ADDR_CNTL5, CNTL5_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL5 successful \n");
    } else {
        printf("AK9756: CNTL5 failed \n");
    }
    
    // // Configure CNTL6 for FIFO defaults
    // // Write to the CNTL6 register to configure the FIFO (0x18, 0x00)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL6, CNTL6_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL6 successful \n");
    } else {
        printf("AK9756: CNTL6 failed \n");
    }
    
    // // Configure CNTL7 for Summation detection w/10-second TC. (0x19, 0x17)
    // // Write to the CNTL7 register to configure the detection
    ret = write_register(&context, AK9756_REG_ADDR_CNTL7, CNTL7_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL7 successful \n");
    } else {
        printf("AK9756: CNTL7 failed \n");
    }
    
    // // Configure CNTL8 for a detection count of 1 (0x1A, 0x00)
    // // Write to the CNTL8 register to configure the detection count
    ret = write_register(&context, AK9756_REG_ADDR_CNTL8, CNTL8_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL8 successful \n");
    } else {
        printf("AK9756: CNTL8 failed \n");
    }
    
    // // Configure CNTL9 for summation departure detection (0x1B, ???)
    // // Write to the CNTL9 register to set departure detection
    ret = write_register(&context, AK9756_REG_ADDR_CNTL9, CNTL9_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL9 successful \n");
    } else {
        printf("AK9756: CNTL9 failed \n");
    }

    // // Configure CNTL10 for a departure detection time of 4s (M=1, N=2) (0x1C, 0x41)
    // // Write to the CNTL10 register to configure the departure detection time
    ret = write_register(&context, AK9756_REG_ADDR_CNTL10, CNTL10_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL10 successful \n");
    } else {
        printf("AK9756: CNTL10 failed \n");
    }
    
    // // Configure CNTL11 through CNTL18 threshold registers. These are initially (0x1D, 0x96)
    // // being set to hex values but will be changed to parameters once it works.
    ret = write_register(&context, AK9756_REG_ADDR_CNTL11, CNTL11_DIFTHLL_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL11 successful \n");
    } else {
        printf("AK9756: CNTL11 failed \n");
    }

    // // Configure CNTL12 (0x1E, 0x00)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL12, CNTL12_DIFTHLU_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL12 successful \n");
    } else {
        printf("AK9756: CNTL12 failed \n");
    }

    // // Configure CNTL13 (0x1F, 0x96)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL13, CNTL13_DIFTHHL_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL13 successful \n");
    } else {
        printf("AK9756: CNTL13 failed \n");
    }

    // // Configure CNTL14 (0x20, 0x00)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL14, CNTL14_DIFTHHU_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL14 successful \n");
    } else {
        printf("AK9756: CNTL14 failed \n");
    }

    // // Configure CNTL15 (0x21, 0x96)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL15, CNTL15_SUMTHLL_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL15 successful \n");
    } else {
        printf("AK9756: CNTL15 failed \n");
    }

    // // Configure CNTL16 (0x22, 0x00)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL16, CNTL16_SUMTHLU_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL16 successful \n");
    } else {
        printf("AK9756: CNTL16 failed \n");
    }

    // // Configure CNTL17 (0x23, 0x96)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL17, CNTL17_SUMTHHL_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL17 successful \n");
    } else {
        printf("AK9756: CNTL17 failed \n");
    }

    // // Configure CNTL18 (0x24, 0x00)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL18, CNTL18_SUMTHHU_DEFAULT );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL18 successful \n");
    } else {
        printf("AK9756: CNTL18 failed \n");
    }

    // // Write to the CNTL19 register to set the device into continuous mode (0x25, 0xFF)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL19, CNTL19_MODE_CONTINUOUS );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL19 successful \n");
    } else {
        printf("AK9756: CNTL19 failed \n");
    }

    // process_start(&ak9756_proc, NULL);
    PROCESS_END();
}