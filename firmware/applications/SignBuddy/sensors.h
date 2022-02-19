#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

void sensors_task_setup(void);
void sensors_task_start(void);
void sensors_sampling_timer_start(void);

void sensors_sample_periodic(uint8_t);

#endif // SENSORS_H
