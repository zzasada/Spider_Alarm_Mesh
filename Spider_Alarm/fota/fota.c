#include "fota.h"
#include "stdbool.h"

PROCESS(fota_proc, "FOTA process");

//TODO: lots

PROCESS_THREAD(fota_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("FOTA, Starting process\n");

    while (true) {
        // printf("FOTA: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}