#include "self_test.h"
#include <mira.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

PROCESS(selftest_proc, "Self Test process");

PROCESS_THREAD(selftest_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("SELF_TEST: Starting self test\n");
    
    printf("SELF_TEST: TODO: set led blue.\n");
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    printf("SELF_TEST: TODO: confirm A111 works\n");
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    printf("SELF_TEST: TODO: confirm Alarm works.\n");
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    printf("SELF_TEST: TODO: confirm LIS3DH works.\n");
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    printf("SELF_TEST: TODO: confirm BQ35100 works.\n");    
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    printf("SELF_TEST: TODO: set led green if success, set led red if fail.");
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    printf("SELF_TEST: TODO: remove ndef device name.\n");    
    printf("SELF_TEST: Test complete.\n");

    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    PROCESS_END()
}