#ifndef FLEX_H
#define FLEX_H

#include <stdint.h>

#include "SignBuddy.pb.h"

/* Initialize flex module */
void flex_task_setup(void);
void flex_task_start(void);
void flex_start_read(void);

/* DMA ADC data transfer complete callback */
void adc_data_ready_cb(void);

/* Register data ready callback */
void flex_callback_register(void (*callback)(void));

/* Get flex sensor data */
void flex_data_get(Sample_FlexData *);

#endif // FLEX_H
