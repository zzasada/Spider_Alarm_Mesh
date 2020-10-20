#include "ak9756.h"
#include "stdbool.h"
#include <mira.h>
#include "common.h"
#include "ak9756_reg.h"

PROCESS(ak9756_setup_proc, "AK9756 Setup Process");
PROCESS(ak9756_pull_proc, "AK9756 process");
PROCESS(ak9756_int_proc, "AK9756 Interrupt process");
PROCESS(ak9756_sleep_proc, "AK9756 Sleep process");

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

static mira_status_t read_register(mira_i2c_context_t *context, uint8_t register_address, uint8_t *byte_array, uint8_t bytes_to_read){
        mira_i2c_set_transfer_type_read(context, &register_address, bytes_to_read, byte_array, bytes_to_read);
        mira_i2c_set_slave_address(context, AK9756_SLAVEADDR);
        return mira_i2c_transfer_start(context);
}

static mira_status_t write_register(mira_i2c_context_t *context, uint8_t register_address, uint8_t command){
        uint8_t dst_local[2];
        dst_local[0] = register_address;
        dst_local[1] = command;
        mira_i2c_set_transfer_type_write(context, &dst_local[0], 2);
        mira_i2c_set_slave_address(context, AK9756_SLAVEADDR);
        return mira_i2c_transfer_start(context);
}

void printDouble(double v, int decimalDigits)
{
  int i = 1;
  int intPart, fractPart;
  for (;decimalDigits!=0; i*=10, decimalDigits--);
  intPart = (int)v;
  fractPart = (int)((v-(double)(int)v)*i);
  if(fractPart < 0) fractPart *= -1;
  printf("%i.%i", intPart, fractPart);
}

PROCESS_THREAD(ak9756_sleep_proc, ev, data){
    
    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("AK9756 sleep thread begin.\n");

    alert_setup();

    while (1) {
            PROCESS_WAIT_UNTIL(mira_gpio_has_triggered_edge_event(ALERT));
            printf("AK9756 motion detected!!!!!!!!!\n");
            process_start(&ak9756_pull_proc, NULL);
            //TODO: inform alarm process
            //TODO: inform network process
    }

    PROCESS_END();
}

PROCESS_THREAD(ak9756_int_proc, ev, data){
    static mira_i2c_context_t sleep_context;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("AK9756 int thread begin.\n");

    init_i2c(&sleep_context);

    // Write to the CNTL19 register to set the device into sleep mode (0x25, 0xFE)
    mira_status_t ret = write_register(&sleep_context, AK9756_REG_ADDR_CNTL19, CNTL19_MODE_STANDBY );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&sleep_context));
        printf("AK9756: CNTL19 successful \n");
    } else {
        printf("AK9756: CNTL19 failed \n");
    }

    //TODO: send command to put sensor to sleep

    PROCESS_END();
}

PROCESS_THREAD(ak9756_pull_proc, ev , data){

    static struct etimer ak9756_timer;
    static mira_i2c_context_t main_context;
    static uint8_t byte_array[10];
    static mira_status_t ret;

    // static uint8_t acquisitionState = ACQUIRE_MODE_IDLE;
    // static uint8_t fifoFull = FALSE;
    // static uint8_t intSource01 = 0;
    // static uint8_t intSource10 = 0;
    static uint8_t Present=0;
    static uint8_t Absent=0;
    static uint8_t AbsentShifted = 0;
    // static uint8_t drdy = 0;
    static uint8_t HBDresult = 0;
    // static uint8_t dataOverrun = 0;
    // static uint8_t readStatus;
    // static uint8_t result = FALSE;

    uint8_t st1Value = 0;
    uint8_t st2Value = 0;

    int16_t ir1Value = 0;
    uint8_t ir1l = 0;
    uint8_t ir1h = 0;

    int16_t ir2Value = 0;
    uint8_t ir2l = 0;
    uint8_t ir2h = 0;

    int16_t tempValue = 0;
    uint8_t templ = 0;
    uint8_t temph = 0;

    // uint8_t tempAcqState = ACQUIRE_MODE_IDLE;
    // uint8_t prevAcqState = ACQUIRE_MODE_IDLE;
    // uint8_t acquireChange = 0;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("AK9756 main thread begin.\n");

    init_i2c(&main_context);

    while (1) {
        etimer_set(&ak9756_timer, 1000); //Wait 1s and pull again
        PROCESS_WAIT_UNTIL(etimer_expired(&ak9756_timer));

        ret = read_register(&main_context, AK9756_REG_ADDR_ST1, &byte_array[0], 8);
        if(ret == MIRA_SUCCESS) {
            PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&main_context));
            // printf("AK9756: Requesting interrupt reason,\n st1Value:%02X\n ir1l:%02X\n ir1h:%02X\n ir2l:%02X\n ir2h:%02X\n templ:%02X\n temph:%02X\n st2Value:%02X\n", byte_array[0], byte_array[1], byte_array[2],byte_array[3], byte_array[4], byte_array[5], byte_array[6], byte_array[7]); 
            // printf("AK9756:\n st1Value:%i\n ir1l:%i\n ir1h:%i\n ir2l:%i\n ir2h:%i\n templ:%i\n temph:%i\n st2Value:%i\n", byte_array[0], byte_array[1], byte_array[2],byte_array[3], byte_array[4], byte_array[5], byte_array[6], byte_array[7]); 
        } else {
            printf("AK9756: Error with I2C transfer.\n");
        }

        st1Value = byte_array[0];
        ir1h = byte_array[1];
        ir1l = byte_array[2];
        ir2h = byte_array[3];
        ir2l = byte_array[4];
        temph = byte_array[5];
        templ = byte_array[6];
        st2Value = byte_array[7];

        Present = st1Value & HPD01mask;
        Absent = st1Value & HPD10mask;
        AbsentShifted = (Present >> 1);
        // drdy = st1Value & drdyMask;
            
        HBDresult = st2Value & HRSLTmask;
        // dataOverrun = st2Value & DORmask;

        ir1Value=(ir1h << 8) | ir1l;
        ir2Value=(ir2h << 8) | ir2l;
        tempValue=(temph << 8) | templ;
        double temperature = 0.0019837 * tempValue + 25;

        if (Present){ // If Present is TRUE
            // printf("Human Detected! IR1=%6d IR2=%6d TEMP=%6d, Temperature=", ir1Value, ir2Value, tempValue);
            printf("Human Detected! IR1=0x%02X%02X/%6d IR2=0x%02X%02X/%6d TEMP=0x%02X%02X, Temperature=", ir1h, ir1l, ir1Value, ir2h, ir2l, ir2Value, temph,templ);
            printDouble(temperature, 2);
            printf("\n");
        }else if (Absent){ // If Absent is TRUE
            // printf("Human Left! IR1=%6d IR2=%6d TEMP=%6d, Temperature=", ir1Value, ir2Value, tempValue);
            printf("Human Left! IR1=0x%02X%02X/%6d IR2=0x%02X%02X/%6d TEMP=0x%02X%02X, Temperature=", ir1h, ir1l, ir1Value, ir2h, ir2l, ir2Value, temph,templ);
            printDouble(temperature, 2);
            printf("\n");
        }else{
            printf("Error detected!");
            printf(" AK9756 ERROR Detected. HRSLT does not agree with the interrupt source \n");
            printf(" HRSLT = %d, Present = %d, AbsentShifted = %d \n", HBDresult, Present, AbsentShifted);
            printf(" Returning to ACQUIRE_MODE_IDLE state \n"); 
        }

        for(int i=0; i<sizeof(byte_array); i++){
            byte_array[i] = 0;
        }

    }
    mira_i2c_uninit(&main_context);//Release the I2C bus
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
    ret = read_register(&context, AK9756_REG_ADDR_WIA1, &byte_array[0], 1);
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
        printf("AK9756: write CNTL2 successful \n");
    } else {
        printf("AK9756: write CNTL2 failed \n");
    }

    ret = read_register(&context, AK9756_REG_ADDR_CNTL2, &byte_array[0], 1);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        if(byte_array[0] == CNTL2_DEMO2){
            printf("AK9756: CNTL2 confirmed \n");
        }else{
            printf("AK9756: Unable to confirm CNTL2, %02X != %02x",byte_array[0], CNTL2_DEMO2);
        }
    } else {
        printf("AK9756: read CNTL2 failed \n");
    }
    
    // // Set up the CNTL3 for ODR=10Hz and TMP & IR Fc = ODR/6 (0x15, 0xAA)
    ret = write_register(&context, AK9756_REG_ADDR_CNTL3, CNTL3_DEMO_10HZ);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: write CNTL3 successful \n");
    } else {
        printf("AK9756: write CNTL3 failed \n");
    }

    ret = read_register(&context, AK9756_REG_ADDR_CNTL3, &byte_array[0], 1);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        if(byte_array[0] == CNTL3_DEMO_10HZ){
            printf("AK9756: CNTL3 confirmed \n");
        }else{
            printf("AK9756: Unable to confirm CNTL3, %02X != %02x",byte_array[0], CNTL3_DEMO_10HZ);
        }
    } else {
        printf("AK9756: read CNTL3 failed \n");
    }
    
    // // Set up the CNTL4 register for Demo mode HD01E, HD10E, HDIE (0x16, 0xF0)
    printf("AK9756: writing register:%02X, command:%02X\n",AK9756_REG_ADDR_CNTL4, CNTL4_HPD_DEMO);
    ret = write_register(&context, AK9756_REG_ADDR_CNTL4, CNTL4_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: write CNTL4 successful \n");
    } else {
        printf("AK9756: write CNTL4 failed \n");
    }

    ret = read_register(&context, AK9756_REG_ADDR_CNTL4, &byte_array[0], 1);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        if(byte_array[0] == CNTL4_HPD_DEMO){
            printf("AK9756: CNTL4 confirmed \n");
        }else{
            printf("AK9756: Unable to confirm CNTL4, %02X != %02x",byte_array[0], CNTL4_HPD_DEMO);
        }
    } else {
        printf("AK9756: read CNTL4 failed \n");
    }

    // // Configure CNTL5 for Bypass Mode
    // // Write to the CNTL5 register to configure the FIFO (0x17, 0xC0)
    printf("AK9756: writing register:%02X, command:%02X\n",AK9756_REG_ADDR_CNTL5, CNTL5_HPD_DEMO);
    ret = write_register(&context, AK9756_REG_ADDR_CNTL5, CNTL5_HPD_DEMO );
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        printf("AK9756: CNTL5 write successful \n");
    } else {
        printf("AK9756: CNTL5 write failed \n");
    }

    ret = read_register(&context, AK9756_REG_ADDR_CNTL5, &byte_array[0], 1);
    if(ret == MIRA_SUCCESS) {
        PROCESS_WAIT_UNTIL(!mira_i2c_transfer_in_progress(&context));
        if(byte_array[0] == CNTL5_HPD_DEMO){
            printf("AK9756: CNTL5 confirmed \n");
        }else{
            printf("AK9756: Unable to confirm CNTL5, %02X != %02x",byte_array[0], CNTL5_HPD_DEMO);
        }
    } else {
        printf("AK9756: read CNTL5 failed \n");
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

    mira_i2c_uninit(&context);//Release the I2C bus

    process_start(&ak9756_int_proc, NULL);
    // process_start(&ak9756_pull_proc, NULL);
    PROCESS_END();
}