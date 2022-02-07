#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

void sensors_task_setup(void);
void sensors_task_start(void);

extern TimerHandle_t sampling_timer;

#endif // SENSORS_H
