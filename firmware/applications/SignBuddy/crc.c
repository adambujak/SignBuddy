#include "crc.h"

#include "board.h"

void crc_init(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);
  CRC->CR = LL_CRC_OUTDATA_REVERSE_NONE | LL_CRC_INDATA_REVERSE_NONE | LL_CRC_POLYLENGTH_32B;
  CRC->POL = LL_CRC_DEFAULT_CRC32_POLY;
  CRC->INIT = LL_CRC_DEFAULT_CRC_INITVALUE;
}

uint32_t crc_compute(uint8_t *buffer, uint8_t buffer_size)
{
  LL_CRC_ResetCRCCalculationUnit(CRC);
  register uint32_t data = 0;
  register uint32_t index = 0;

  for (index = 0; index < (buffer_size / 4); index++) {
    data =
      (uint32_t) ((buffer[4 * index + 3] << 24) | (buffer[4 * index + 2] << 16) | (buffer[4 * index + 1] << 8) |
                  buffer[4 * index]);
    LL_CRC_FeedData32(CRC, data);
    __NOP();
    __NOP();
    __NOP();
    __NOP();
  }

  switch (buffer_size % 4) {
  case 1:
    LL_CRC_FeedData8(CRC, buffer[4 * index]);
    __NOP();
    break;

  case 2:
    LL_CRC_FeedData16(CRC, (uint16_t) ((buffer[4 * index + 1] << 8) | buffer[4 * index]));
    __NOP();
    __NOP();
    break;

  case 3:
    LL_CRC_FeedData16(CRC, (uint16_t) ((buffer[4 * index + 1] << 8) | buffer[4 * index]));
    __NOP();
    __NOP();
    LL_CRC_FeedData8(CRC, buffer[4 * index + 2]);
    __NOP();
    break;
  }

  return LL_CRC_ReadData32(CRC);
}
