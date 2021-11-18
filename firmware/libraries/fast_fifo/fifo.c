#include "fifo.h"

#include <stdbool.h>

#define IS_POWER_OF_TWO(num)    (((num) & ((num) - 1)) == 0) ? true : false

int fifo_init(fifo_t *fifo, uint8_t *buffer, uint32_t size)
{
  if (!(IS_POWER_OF_TWO(size))) {
    return -1;
  }

  fifo->buffer = buffer;
  fifo->size = size;
  fifo->write_index = 0;
  fifo->read_index = 0;
  fifo->bytes_used = 0;
  return 0;
}

int fifo_push(fifo_t *fifo, uint8_t *buffer, uint32_t length)
{
  if ((fifo->bytes_used + length) > fifo->size) {
    return 0;
  }

  for (uint32_t i = 0; i < length; i++) {
    fifo->buffer[fifo->write_index] = buffer[i];
    fifo->write_index = (fifo->write_index + 1) & (fifo->size - 1);
  }

  fifo->bytes_used += length;
  return length;
}

int fifo_pop(fifo_t *fifo, uint8_t *dest, uint32_t length)
{
  if (length > fifo->bytes_used) {
    return 0;
  }

  for (uint32_t i = 0; i < length; i++) {
    dest[i] = fifo->buffer[fifo->read_index];
    fifo->read_index = (fifo->read_index + 1) & (fifo->size - 1);
  }
  fifo->bytes_used -= length;
  return length;
}

int fifo_peek(fifo_t *fifo, uint8_t *dest, uint32_t length)
{
  if (length > fifo->bytes_used) {
    return 0;
  }
  uint32_t read_index = fifo->read_index;

  for (uint32_t i = 0; i < length; i++) {
    dest[i] = fifo->buffer[read_index];
    read_index = (read_index + 1) & (fifo->size - 1);
  }
  return length;
}

int fifo_drop(fifo_t *fifo, uint32_t length)
{
  if (fifo->bytes_used < length) {
    return 0;
  }

  fifo->read_index = (fifo->read_index + length) & (fifo->size - 1);
  fifo->bytes_used = fifo->bytes_used - length;

  return length;
}

uint32_t fifo_bytes_used_cnt_get(fifo_t *fifo)
{
  return fifo->bytes_used;
}

uint32_t fifo_bytes_unused_cnt_get(fifo_t *fifo)
{
  return fifo->size - fifo->bytes_used;
}
