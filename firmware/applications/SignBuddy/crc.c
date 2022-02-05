#include "crc.h"

#include "board.h"

void init_crc(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);
}

uint32_t compute_crc(uint8_t *buffer, uint8_t buffer_size)
{
  register uint32_t data = 0;
  register uint32_t index = 0;

  for (index = 0; index < (buffer_size / 4); index++) {
    data =
      (uint32_t) ((buffer[4 * index + 3] << 24) | (buffer[4 * index + 2] << 16) | (buffer[4 * index + 1] << 8) |
                  buffer[4 * index]);
    LL_CRC_FeedData32(CRC, data);
  }

  switch (buffer_size % 4) {
  case 1:
    LL_CRC_FeedData8(CRC, buffer[4 * index]);
    break;

  case 2:
    LL_CRC_FeedData16(CRC, (uint16_t) ((buffer[4 * index + 1] << 8) | buffer[4 * index]));
    break;

  case 3:
    LL_CRC_FeedData16(CRC, (uint16_t) ((buffer[4 * index + 1] << 8) | buffer[4 * index]));
    LL_CRC_FeedData8(CRC, buffer[4 * index + 2]);
    break;
  }

  return LL_CRC_ReadData32(CRC);
}
