#include "logger.h"
#include "log_uart.h"

#include <string.h>

void logger_write(char *string)
{
  size_t length = strlen(string);

  if (length > LOG_MAX_STRING_LEN) {
    string[length - 1] = '\0';
  }

  log_uart_write((uint8_t *) string, (uint32_t) length);
}
