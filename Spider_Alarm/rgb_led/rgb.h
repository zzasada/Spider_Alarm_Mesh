#ifndef RGB_H_
#define RGB_H_

#include <mira.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum color {BLACK, BLUE, GREEN, RED, CYAN, MAGENTA, YELLOW, WHITE};

void init_rgb(void);
void set_current_color(int color);
PROCESS_NAME(rgb_proc);

#endif