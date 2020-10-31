#include <mira.h>
#include <stdio.h>

#include "config/app-config.h"
#include "app/application.h"
#include "nfc/nfc-if.h"
#include "self_test/self_test.h"
#include "rgb_led/rgb.h"

PROCESS(boot_proc, "Boot process");

MIRA_IODEFS(
    MIRA_IODEF_NONE,    /* fd 0: stdin */
    MIRA_IODEF_RTT(0),
    MIRA_IODEF_NONE     /* fd 2: stderr */
    /* More file descriptors can be added, for use with dprintf(); */
);

void mira_setup(void){
    mira_status_t rtt_ret;
    rtt_ret = mira_rtt_init();
    if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }

    MIRA_MEM_SET_BUFFER(12288);

    init_rgb();
    nfcif_init();
    process_start(&boot_proc, NULL);
}

PROCESS_THREAD(boot_proc, ev, data)
{
    // static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();


    app_config_init();

    int configured = app_name_is_configured();
    printf("BOOT: configured = %i\n",configured);

    if(configured == CONFIGURED) {
        printf("BOOT: Configured, starting\n");
        process_start(&main_proc, NULL);
    }else if(configured == TEST){ 
        printf("BOOT: run self test\n");
        process_start(&selftest_proc, NULL);
        // erase_device_name();
        // mira_sys_reset();
    }else {
        printf("BOOT: Not configured, sleeping\n");
    }

    while (1) {
        etimer_set(&timer, 10*CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}
