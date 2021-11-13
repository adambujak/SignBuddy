#ifndef FAST_FIFO_H
#define FAST_FIFO_H

#include <stdint.h>

typedef struct
{
  uint8_t *buffer;
  uint32_t write_index;
  uint32_t read_index;
  uint32_t bytes_available;
  uint32_t size;
} fifo_t;

/** Initialize fifo
 * @param  fifo - pointer to fifo
 * @param  buffer - where to store data in fifo
 * @param  size - size of fifo
 * @retval success: 0, fail: -1
 */
int fifo_init(fifo_t *fifo, uint8_t *buffer, uint32_t size);

/** Push data to fifo
 * @param  fifo - pointer to fifo
 * @param  buffer - data to add to fifo
 * @param  length - number of bytes to add to fifo
 * @retval None
 */
void fifo_push(fifo_t *fifo, uint8_t *buffer, uint32_t length);

/** Pop data out of fifo
 * @param  fifo - pointer to fifo
 * @param  dest - where to write data from fifo
 * @param  length - number of bytes to pop
 * @retval number of bytes popped
 */
int fifo_pop(fifo_t *fifo, uint8_t *dest, uint32_t length);

/** Read data out of fifo without removing data
 * @param  fifo - pointer to fifo
 * @param  dest - where to write data from fifo
 * @param  length - number of bytes to read
 * @retval number of bytes read
 */
int fifo_peek(fifo_t *fifo, uint8_t *dest, uint32_t length);

/** Drop data from fifo
 * @param  fifo - pointer to fifo
 * @param  length - number of bytes to drop
 * @retval None
 */
void fifo_drop(fifo_t *fifo, uint32_t length);

/** Get bytes available in fifo
 * @param  fifo - pointer to fifo
 * @retval number of bytes in fifo
 */
uint32_t fifo_available(fifo_t *fifo);

#endif // FAST_FIFO_H
