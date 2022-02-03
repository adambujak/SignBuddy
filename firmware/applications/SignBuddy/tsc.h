#ifndef TSC_H
#define TSC_H

#include <stdint.h>


/* Initialize tsc module */
void tsc_init(void);

uint32_t tsc_get_value(void);
void tsc_config(void);

#endif // TSC_H
