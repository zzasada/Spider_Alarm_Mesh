#include "network.h"
#include "stdbool.h"

PROCESS(network_proc, "Network process");

//TODO: initialize network
//TODO: wait for name to be set by nfc
//TODO: connect to network
//TODO: report status to led, alarm, and battery monitor

PROCESS_THREAD(network_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("Network, Starting process\n");

    while (true) {
        // printf("Network: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}