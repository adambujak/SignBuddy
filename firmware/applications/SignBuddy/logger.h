#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

void logger_write(char *string);

#define LOG_MAX_STRING_LEN 256

#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_WARN  3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_NONE  5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif // LOG_LEVEL

#ifndef LOG_MODE_BLOCKING
#define LOG_MODE_BLOCKING 1
#endif // LOG_MODE_BLOCKING

#define LOGX(fmt, ...)                   \
  do {                                   \
    char str[LOG_MAX_STRING_LEN];        \
    sprintf(str, (fmt), ##__VA_ARGS__);  \
    logger_write(str);                   \
  } while (0)

#if (LOG_LEVEL <= LOG_LEVEL_DEBUG)
#define LOG_DEBUG(fmt, ...)  LOGX((fmt), ##__VA_ARGS__)
#else
#define LOG_DEBUG(...) do {} while (0)
#endif // LOG_LEVEL == LOG_LEVEL_DEBUG

#if (LOG_LEVEL <= LOG_LEVEL_INFO)
#define LOG_INFO(fmt, ...)  LOGX((fmt), ##__VA_ARGS__)
#else
#define LOG_INFO(...) do {} while (0)
#endif // LOG_LEVEL == LOG_LEVEL_INFO

#if (LOG_LEVEL <= LOG_LEVEL_WARN)
#define LOG_WARN(fmt, ...)  LOGX((fmt), ##__VA_ARGS__)
#else
#define LOG_WARN(...) do {} while (0)
#endif // LOG_LEVEL == LOG_LEVEL_WARN

#if (LOG_LEVEL <= LOG_LEVEL_ERROR)
#define LOG_ERROR(fmt, ...)  LOGX((fmt), ##__VA_ARGS__)
#else
#define LOG_ERROR(...) do {} while (0)
#endif // LOG_LEVEL == LOG_LEVEL_ERROR

#endif // LOGGER_H
