#include "alarm.h"
#include "stdbool.h"

PROCESS(alarm_proc, "Alarm process");

void init_alarm(void){
    //TODO: intialize pwm and pins for alarm
    printf("Alarm setup called");
}

//TODO: lots

PROCESS_THREAD(alarm_proc, ev , data)
{
    static struct etimer timer;

    PROCESS_BEGIN();
    PROCESS_PAUSE();
    
    printf("Alarm, Starting process\n");

    while (true) {
        // printf("Alarm: hello\n");
        
        etimer_set(&timer, CLOCK_SECOND);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }
    PROCESS_END()
}