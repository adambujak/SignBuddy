#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#include <stdint.h>

/** Get difference between two system times in ticks
 * @param  old_time - first system time - (in ticks)
 * @param  new_time - second system time - (in ticks)
 * @retval Difference between times in ticks
*/
uint32_t system_time_cmp_ticks(uint32_t old_time, uint32_t new_time);

/** Get difference between two system times in us
 * @param  old_time - first system time - (in ticks)
 * @param  new_time - second system time - (in ticks)
 * @retval Difference between times in us
*/
uint32_t system_time_cmp_us(uint32_t old_time, uint32_t new_time);

/** Get difference between two system times in ms
 * @param  old_time - first system time - (in ticks)
 * @param  new_time - second system time - (in ticks)
 * @retval Difference between times in ms
*/
uint32_t system_time_cmp_ms(uint32_t old_time, uint32_t new_time);

/** Get timer's cnt
 * @retval Tick count of system time timer
*/
uint32_t system_time_get(void);

/* Initialize system_time module */
void system_time_init(void);

#endif // SYSTEM_TIME_H