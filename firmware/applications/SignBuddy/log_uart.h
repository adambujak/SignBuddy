#ifndef LOG_UART_H
#define LOG_UART_H

#include <stdint.h>
#include <stdbool.h>

#define LOG_UART_TX_BUFFER_SIZE    512
#define LOG_UART_RX_BUFFER_SIZE    512

void log_uart_init(void);
void log_uart_write(uint8_t *data, uint32_t length);
int log_uart_read(uint8_t *data, uint32_t length);
bool log_uart_is_writing(void);

#endif  // LOG_UART_H
