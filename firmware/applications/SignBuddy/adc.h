#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/** Read data from the adc
 * @retval    Data from the adc in mVolt
 */
uint16_t adc_read(void);

/* Initialize adc module */
void adc_init(void);

/* Enable adc module */
void adc_enable(void);

/* Disable adc module */
void adc_disable(void);

#endif // ADC_H
