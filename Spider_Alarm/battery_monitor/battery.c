#include "battery.h"
#include "stdbool.h"

PROCESS(battery_proc, "Battery process");

//TODO: lots

PROCESS_THREAD(battery_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("Battery, Starting process\n");

    while (true) {
        // printf("Battery: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}