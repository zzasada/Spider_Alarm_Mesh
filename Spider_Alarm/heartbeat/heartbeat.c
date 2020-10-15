#include "heartbeat.h"
#include "stdbool.h"

PROCESS(heartbeat_proc, "Heartbeat process");

//TODO: lots

PROCESS_THREAD(heartbeat_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("Heartbeat, Starting process\n");

    while (true) {
        // printf("Heartbeat: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}