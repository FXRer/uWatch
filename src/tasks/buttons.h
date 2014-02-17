#ifndef BUTTON_H__
#define BUTTON_H__

#include "../cocoOS/cocoos.h"

extern Evt_t buttonEvent;

void button_task(void);

char button_get_pressed(void);
char button_get_hold_duration(void);



#endif

