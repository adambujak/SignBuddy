#ifndef TSC_H
#define TSC_H

#include <stdint.h>

#define TOUCH_SENSOR_CNT    3

/* Initialize tsc module */
void tsc_task_setup(void);
void tsc_task_start(void);
void tsc_start_read(void);

// register data ready callback
void tsc_callback_register(void (*callback)(void));

void tsc_get_value(int8_t touch_values[TOUCH_SENSOR_CNT]);

#endif // TSC_H
