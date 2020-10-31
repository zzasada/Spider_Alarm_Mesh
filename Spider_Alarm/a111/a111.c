#include "a111.h"
#include <mira.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

PROCESS(a111_proc, "A111 process");
PROCESS(a111_setup_proc, "A111 process");

PROCESS_THREAD(a111_setup_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("A111: Setup started (TODO: finish setup)\n");
    
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    printf("A111: Setup complete");

    PROCESS_END()
}

PROCESS_THREAD(a11_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("A111: Process (TODO: finish implementation.)\n");

    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    PROCESS_END()
}