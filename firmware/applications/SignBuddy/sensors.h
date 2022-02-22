#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"

typedef enum
{
  SAMPLE_STATIC,
  SAMPLE_DYNAMIC
} sample_method_t;

void sensors_task_setup(void);
void sensors_task_start(void);

void sensors_sample(sample_method_t method);

#endif // SENSORS_H
