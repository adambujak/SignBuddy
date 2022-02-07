#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

void sensors_task_setup(void);
void sensors_task_start(void);
void sensors_sampling_timer_start(void);

#endif // SENSORS_H
