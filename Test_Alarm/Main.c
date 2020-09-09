//TODO: intialize SPI
//TODO: implement LIS3DH

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "pwm/pwm_lib.h"

#define ALARM_INTERVAL 1

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

void alarm_on(void){
    printf("alarm_on()\n");
    mira_gpio_set_value(ALARM_ENABLE, 1);
}

void alarm_off(void){
    printf("alarm_off()\n");
    pwm_set_value(0);

    mira_gpio_set_value(ALARM_ENABLE, 0);
}

PROCESS(main_proc, "Main process");

void pin_setup(void){
    pwm_set_value(0);
    pwm_enable();
    
    mira_gpio_set_dir(ALARM_ENABLE, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(ALARM_ENABLE, 0);
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

    printf("Starting Test Alarm.\n");
    pwm_init();
    alarm_on();

    //Give 10ms for boost to power up.
    etimer_set(&timer, 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    pwm_set_value(50);

    etimer_set(&timer, ALARM_INTERVAL * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    alarm_off();

    while (1) {

        // alarm_on();
        // etimer_set(&timer, ALARM_INTERVAL * CLOCK_SECOND);
        // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // alarm_off();
        etimer_set(&timer, ALARM_INTERVAL * CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }


    PROCESS_END();
}
