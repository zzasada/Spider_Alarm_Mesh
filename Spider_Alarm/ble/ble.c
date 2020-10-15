#include "ble.h"
#include "stdbool.h"

PROCESS(ble_proc, "BLE process");

//TODO: lots

PROCESS_THREAD(ble_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("BLE, Starting process\n");

    while (true) {
        // printf("BLE: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}