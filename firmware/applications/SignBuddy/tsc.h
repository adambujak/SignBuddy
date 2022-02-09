#ifndef TSC_H
#define TSC_H

#include <stdint.h>

#include <SignBuddy.pb.h>

#define TOUCH_SENSOR_CNT    12

/* Initialize tsc module */
void tsc_task_setup(void);
void tsc_task_start(void);
void tsc_start_read(void);

// register data ready callback
void tsc_callback_register(void (*callback)(void));

void tsc_data_get(Sample_TouchData *data);

#endif // TSC_H
