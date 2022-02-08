#ifndef LOG_UART_H
#define LOG_UART_H

#include <stdint.h>
#include <stdbool.h>

void log_uart_init(void);
void log_uart_write(uint8_t *data, uint32_t length);
int log_uart_read(uint8_t *data, uint32_t length);

#endif  // LOG_UART_H
