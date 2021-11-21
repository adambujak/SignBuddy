#include "logger.h"
#include "log_uart.h"

#include <string.h>

void logger_write(char *string)
{
  size_t length = strlen(string);

  if (length > LOG_UART_TX_BUFFER_SIZE) {
    string[length - 1] = '\0';
  }

  log_uart_write((uint8_t *) string, (uint32_t) length);

#if LOG_MODE_BLOCKING
  while (log_uart_is_writing());
#endif // LOG_MODE_BLOCKING
}
