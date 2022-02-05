#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* Initialize adc module */
void adc_init(void);

/* Enable adc module */
void adc_enable(void);

/* Disable adc module */
void adc_disable(void);

/* Start adc conversion */
void adc_start_conversion(void);

#endif // ADC_H
