#include <mira.h>
#include <stdio.h>

#include "app-config.h"
#include "application.h"

#include "board.h"

PROCESS(boot_proc, "Boot process");

void mira_setup(void)
{
   // MIRA_MEM_SET_BUFFER(15000);
    board_setup();

    mira_gpio_set_dir(BOARD_LED2_PIN, MIRA_GPIO_DIR_OUT);
    mira_gpio_set_value(BOARD_LED2_PIN, 1);

    process_start(&boot_proc, NULL);
}

PROCESS_THREAD(boot_proc, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();


    app_config_init();

    if(app_config_is_configured()) {
        printf("Configured, starting\n");
        process_start(&main_proc, NULL);
    } else {
        printf("Not configured, sleeping\n");
    }

    while (1) {
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}
