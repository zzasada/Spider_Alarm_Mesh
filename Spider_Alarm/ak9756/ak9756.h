#ifndef AK9756_H_
#define AK9756_H_

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void alert_setup(void);
PROCESS_NAME(ak9756_proc);
PROCESS_NAME(ak9756_setup_proc);

#endif