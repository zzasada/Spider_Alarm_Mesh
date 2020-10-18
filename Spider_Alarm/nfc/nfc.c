#include "nfc.h"
#include "stdbool.h"

PROCESS(nfc_proc, "NFC process");

//TODO: notify network process when name is available
//TODO: save name in shared location
//TODO: reboot when name is cleared

PROCESS_THREAD(nfc_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("NFC, Starting process\n");

    while (true) {
        // printf("NFC: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}