//TODO: intialize SPI
//TODO: implement LIS3DH

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "pwm/pwm_lib.h"

#define LED_INTERVAL 1

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

void led_red(struct etimer *timer){
    printf("led_red()\n");
    mira_gpio_set_value(LED_RED, 0);
    etimer_set(timer, LED_INTERVAL * CLOCK_SECOND);
}

void led_green(){
    printf("led_green()\n");
    mira_gpio_set_value(LED_GREEN, 0);
}

void led_blue(){
    printf("led_blue()\n");
    mira_gpio_set_value(LED_BLUE, 0);
}

void led_demo(){
    printf("led_demo() todo finish with pwm\n");
}

void led_off(){
    printf("led_off()\n");
    mira_gpio_set_value(LED_RED, 1);
    mira_gpio_set_value(LED_GREEN, 1);
    mira_gpio_set_value(LED_BLUE, 1);
}

PROCESS(main_proc, "Main process");

void pin_setup(void){
    // pwm_set_value(0);
    // pwm_enable();
    mira_gpio_set_dir(LED_RED, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_RED, 1);

    mira_gpio_set_dir(LED_GREEN, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_GREEN, 1);

    mira_gpio_set_dir(LED_BLUE, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_BLUE, 1);
}

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    pin_setup();

    process_start(&main_proc, NULL);
}

PROCESS_THREAD(main_proc, ev, data)
{
    static struct etimer timer;
    // static int i=0;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("Starting Test LED.\n");
    // pwm_init();
    // pwm_set_value(100);
    // pwm_enable();

    while (1) {
        led_red(&timer);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        led_off();
        led_green();
        etimer_set(&timer, LED_INTERVAL * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        led_off();
        led_blue();
        etimer_set(&timer, LED_INTERVAL * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        led_off();
        // for(i=0; i<100; i++){
        //     pwm_set_value(i);
        //     etimer_set(&timer, 10);
        //     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        // }

        // for(i=100; i>0; i--){
        //     pwm_set_value(i);
        //     etimer_set(&timer, 10);
        //     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        // }
    }


    PROCESS_END();
}
