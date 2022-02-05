#ifndef FLEX_H
#define FLEX_H

#include <stdint.h>

#define FLEX_SENSOR_CNT    5

/* Initialize flex module */
void flex_task_setup(void);
void flex_task_start(void);
void flex_start_read(void);

/* DMA data transfer complete callback */
void dma_data_ready_cb(void);

/* Register data ready callback */
void flex_callback_register(void (*callback)(void));

/* Get flex sensor value */
void flex_get_value(uint16_t *, uint8_t);

#endif // FLEX_H
