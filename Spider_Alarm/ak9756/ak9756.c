#include "ak9756.h"
#include "stdbool.h"

PROCESS(ak9756_proc, "AK9756 process");

PROCESS_THREAD(ak9756_proc, ev , data)
{
    static struct etimer timer;
    //TODO: capture interrupt
    //TODO: pass in network connection state
    //TODO: intialize AK9756
    //TODO: notify gateway and buzzer if motion is detected
    //TODO: allow disabled mode. (put in lowest power state).

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    printf("AK9756, Starting process\n");

    while (true) {
        // printf("AK9756: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}