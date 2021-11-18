#ifndef COMMON_H
#define COMMON_H

#include "system_time.h"

void error_handler(void);
void delay_ms(uint32_t ms);

#define RET_OK                0
#define RET_ERR               1

#define SYS_TICK_FREQ         1000
#define MS_TO_TICKS(MS)       (MS)
#define TICKS_TO_MS(ticks)    (ticks)

#define ERR_CHECK(x)        \
  do {                      \
    int retval = (x);       \
    if (retval != RET_OK) { \
      error_handler();      \
    }                       \
  } while (0)

#define ASSERT(val)    \
  do {                 \
    if (!(val)) {      \
      error_handler(); \
    }                  \
  } while (0)

#define DISABLE_IRQ()     \
  uint32_t prim;          \
  prim = __get_PRIMASK(); \
  __disable_irq();        \

#define ENABLE_IRQ() \
  if (!prim) {       \
    __enable_irq();  \
  }                  \

#endif // COMMON_H
