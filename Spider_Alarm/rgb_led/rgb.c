#include "rgb.h"
#include "stdbool.h"
#include "common.h"

PROCESS(rgb_proc, "RGB process");

static enum color {BLACK, BLUE, GREEN, RED, CYAN, MAGENTA, YELLOW, WHITE} current_color;

void init_rgb(void){
    printf("RGB: init_rgb\n");
    mira_gpio_set_dir(LED_RED, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_RED, 1);

    mira_gpio_set_dir(LED_GREEN, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_GREEN, 1);

    mira_gpio_set_dir(LED_BLUE, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(LED_BLUE, 1);
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

static void switch_color(int color){
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

PROCESS_THREAD(rgb_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("RGB, Starting process\n");

    while (true) {
        // printf("RGB: hello\n");
        
        switch_color(current_color);
        current_color++;
        if(current_color > WHITE){
            current_color = 0;
        }
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}