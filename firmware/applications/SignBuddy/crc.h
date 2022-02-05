#ifndef CRC_H
#define CRC_H

#include <stdint.h>

void init_crc(void);

uint32_t compute_crc(uint8_t *, uint8_t);

#endif // CRC_H
