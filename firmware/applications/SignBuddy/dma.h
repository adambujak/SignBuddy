#ifndef DMA_H
#define DMA_H

#include <stdint.h>

/* Initialize dma module for adc */
void dma_adc_init(uint32_t, uint32_t);

/* Initialize dma module for comms */
void dma_uart_init(uint32_t, uint32_t, uint32_t, uint32_t);

#endif // DMA_H
