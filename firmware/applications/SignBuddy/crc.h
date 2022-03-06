#ifndef CRC_H
#define CRC_H

#include <stdint.h>

void crc_init(void);

uint32_t crc_compute(uint8_t *, uint8_t);

#endif // CRC_H
