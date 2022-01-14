#ifndef I2C_H
#define I2C_H

#include "board.h"

typedef struct {
  void *handle;
} i2c_t;

void i2c_init(i2c_t *instance, void *hw_i2c_handle, LL_I2C_InitTypeDef *config);
int i2c_write(i2c_t *instance, uint8_t slave_addr, uint8_t reg_addr, const uint8_t *data, uint16_t length);
int i2c_read(i2c_t *instance, uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length);

#endif // I2C_H
