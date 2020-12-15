//TODO: intialize SPI
//TODO: implement LIS3DH

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "pwm/pwm_lib.h"

#define LED_INTERVAL 1
enum color {BLACK, BLUE, GREEN, RED, CYAN, MAGENTA, YELLOW, WHITE};
const int PAUSE_DURATION = 1;
const int PULSE_DURATION = 1000; //mS

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),  /* fd 1: stdout */
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

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

PROCESS(rgb_proc, "RGB process");

static enum color current_color = RED;

void init_rgb(void){
    printf("RGB: init_rgb\n");
    mira_gpio_set_dir(LED_RED, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_RED, 1);

    mira_gpio_set_dir(LED_GREEN, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_GREEN, 1);

    mira_gpio_set_dir(LED_BLUE, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_BLUE, 1);
}

void set_current_color(int color){
    // printf("Setting current color %i\n", color);
    current_color = color;
}

static void red(void){
    mira_gpio_set_value(LED_RED, 0);
    mira_gpio_set_value(LED_GREEN, 1);
    mira_gpio_set_value(LED_BLUE, 1);
}

static void green(void){
    mira_gpio_set_value(LED_RED, 1);
    mira_gpio_set_value(LED_GREEN, 0);
    mira_gpio_set_value(LED_BLUE, 1);
}

static void blue(void){
    mira_gpio_set_value(LED_RED, 1);
    mira_gpio_set_value(LED_GREEN, 1);
    mira_gpio_set_value(LED_BLUE, 0);
}

static void white(void){
    mira_gpio_set_value(LED_RED, 0);
    mira_gpio_set_value(LED_GREEN, 0);
    mira_gpio_set_value(LED_BLUE, 0);
}

static void black(void){
    mira_gpio_set_value(LED_RED, 1);
    mira_gpio_set_value(LED_GREEN, 1);
    mira_gpio_set_value(LED_BLUE, 1);
}

static void magenta(void){
    mira_gpio_set_value(LED_RED, 0);
    mira_gpio_set_value(LED_GREEN, 1);
    mira_gpio_set_value(LED_BLUE, 0);
}

static void yellow(void){
    mira_gpio_set_value(LED_RED, 0);
    mira_gpio_set_value(LED_GREEN, 0);
    mira_gpio_set_value(LED_BLUE, 1);
}

static void cyan(void){
    mira_gpio_set_value(LED_RED, 1);
    mira_gpio_set_value(LED_GREEN, 0);
    mira_gpio_set_value(LED_BLUE, 0);
}

static void set_color(int color){
    switch (color){
    
    case RED:
        red();
        break;

    case GREEN:
        green();
        break;

    case BLUE:
        blue();
        break;

    case MAGENTA:
        magenta();
        break;

    case YELLOW:
        yellow();
        break;

    case CYAN:
        cyan();
        break;

    case WHITE:
        white();
        break;

    case BLACK:
        black();
        break;
    
    default:
        printf("RGB: Invalid color");
        break;
    }
}

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    // pin_setup();
    init_rgb();

    process_start(&rgb_proc, NULL);
}

PROCESS_THREAD(rgb_proc, ev , data){
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("RGB, Starting process\n");

    while (true) {
        // set_color(current_color);
        // etimer_set(&timer, PULSE_DURATION);
        // PROCESS_YIELD_UNTIL(etimer_expired(&timer));

        set_color(RED);
        etimer_set(&timer, PAUSE_DURATION * CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));

        set_color(GREEN);
        etimer_set(&timer, PAUSE_DURATION * CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));

        set_color(BLUE);
        etimer_set(&timer, PAUSE_DURATION * CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}
