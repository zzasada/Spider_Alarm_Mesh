#include "lis2dh.h"
#include "stdbool.h"

PROCESS(lis2dh_proc, "LIS2DH process");

//TODO: intialize lis2dh for tamper detection
//TODO: check if connected to network
//TODO: send tamper message if alarm is tampered with
//TODO: trigger audio if tampered with
//TODO: look for interrupt from accelerometer

PROCESS_THREAD(lis2dh_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("LIS2DH, Starting process\n");

    while (true) {
        // printf("LIS2DH: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}