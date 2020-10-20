#ifndef AK9756_H_
#define AK9756_H_

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0
#define HPD01mask 0x02
#define HPD10mask 0x04
#define drdyMask 0x01
#define HRSLTmask 0x02
#define DORmask 0x01
#define HPD01intTrue 0x02
#define HPD10intTrue 0x04

void alert_setup(void);
PROCESS_NAME(ak9756_pull_proc);
PROCESS_NAME(ak9756_setup_proc);
PROCESS_NAME(ak9756_int_proc);
PROCESS_NAME(ak9756_sleep_proc);

#endif