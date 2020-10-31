#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

#include "ak9756/ak9756.h"
#include "alarm/alarm.h"
#include "battery_monitor/battery.h"
#include "ble/ble.h"
#include "fota/fota.h"
#include "heartbeat/heartbeat.h"
#include "lis2dh/lis2dh.h"
#include "network/network.h"
#include "rgb_led/rgb.h"

PROCESS(main_proc, "Main process");

// void mira_setup(void){
//     mira_status_t rtt_ret;
//     rtt_ret = mira_rtt_init();
//     if(rtt_ret != MIRA_SUCCESS){ /*TODO: figure out how to report this */ }
//     MIRA_MEM_SET_BUFFER(12288);

//     // init_alarm();
//     // init_rgb();

//     process_start(&main_proc, NULL);

//     // process_start(&alarm_proc, NULL);
//     // process_start(&battery_proc, NULL);
//     // process_start(&ble_proc, NULL);
//     // process_start(&fota_proc, NULL);
//     // process_start(&heartbeat_proc, NULL);
//     // process_start(&lis2dh_proc, NULL);
//     // process_start(&network_proc, NULL);
//     // process_start(&nfc_proc, NULL);
//     // process_start(&rgb_proc, NULL);
// }

PROCESS_THREAD(main_proc, ev, data){
    static struct etimer timer;

    PROCESS_BEGIN();
    /* Pause once, so we don't run anything before finish of startup */
    PROCESS_PAUSE();

    printf("MAIN: starting.\n");
    //TODO: wait until we have a name assigned by NFC. Then start trying to connect. 

    while (true){
        // printf("MAIN: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    
    PROCESS_END();
}
