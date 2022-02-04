#include "i2c.h"

#include "common.h"
#include "logger.h"

#define I2C_MAX_TIMEOUT    10

static uint8_t write_byte(I2C_TypeDef *handle, const uint8_t *tx_data)
{
  uint8_t timeout = I2C_MAX_TIMEOUT;

  if (LL_I2C_IsActiveFlag_TXE(handle)) {
    LL_I2C_TransmitData8(handle, *tx_data);
    while (!LL_I2C_IsActiveFlag_TXE(handle) && !LL_I2C_IsActiveFlag_STOP(handle)) {
      if (LL_SYSTICK_IsActiveCounterFlag()) {
        if (timeout-- == 0) {
          LL_I2C_GenerateStopCondition(handle);
          LOG_ERROR("I2C: Timeout on write\r\n");
          return RET_ERR;
        }
      }
    }
  }
  else {
    LOG_ERROR("I2C: Transmit register full\r\n");
    return RET_ERR;
  }

  return RET_OK;
}

static uint8_t read_byte(I2C_TypeDef *handle, uint8_t *rx_data)
{
  uint8_t timeout = I2C_MAX_TIMEOUT;

  while (!LL_I2C_IsActiveFlag_RXNE(handle) && !LL_I2C_IsActiveFlag_STOP(handle)) {
    if (LL_SYSTICK_IsActiveCounterFlag()) {
      if (timeout-- == 0) {
        LL_I2C_GenerateStopCondition(handle);
        LOG_ERROR("I2C: Timeout on read\r\n");
        return RET_ERR;
      }
    }
  }

  if (LL_I2C_IsActiveFlag_RXNE(handle)) {
    *rx_data = LL_I2C_ReceiveData8(handle);
  }
  else {
    LOG_ERROR("I2C: Receive register already read");
    return RET_ERR;
  }

  return RET_OK;
}

uint8_t i2c_write(i2c_t *instance, uint8_t slave_addr, uint8_t reg_addr, const uint8_t *data, uint16_t length)
{
  I2C_TypeDef *handle = (I2C_TypeDef *) instance->handle;

  LL_I2C_HandleTransfer(handle,
                        slave_addr,
                        LL_I2C_ADDRSLAVE_7BIT,
                        length + 1,
                        LL_I2C_MODE_AUTOEND,
                        LL_I2C_GENERATE_START_WRITE);

  ERR_CHECK(write_byte(handle, &reg_addr));

  for (uint32_t i = 0; i < length; i++) {
    ERR_CHECK(write_byte(handle, &data[i]));
  }

  while (!LL_I2C_IsActiveFlag_STOP(handle));
  LL_I2C_ClearFlag_STOP(handle);

  return RET_OK;
}

uint8_t i2c_read(i2c_t *instance, uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length)
{
  I2C_TypeDef *handle = (I2C_TypeDef *) instance->handle;

  i2c_write(instance, slave_addr, reg_addr, NULL, 0);

  LL_I2C_HandleTransfer(handle,
                        slave_addr,
                        LL_I2C_ADDRSLAVE_7BIT,
                        length,
                        LL_I2C_MODE_AUTOEND,
                        LL_I2C_GENERATE_START_READ);

  for (uint32_t i = 0; i < length; i++) {
    ERR_CHECK(read_byte(handle, &data[i]));
  }

  while (!LL_I2C_IsActiveFlag_STOP(handle));
  LL_I2C_ClearFlag_STOP(handle);

  return RET_OK;
}

void i2c_init(i2c_t *instance, void *hw_i2c_handle, LL_I2C_InitTypeDef *config)
{
  LL_I2C_EnableAutoEndMode(hw_i2c_handle);
  LL_I2C_DisableOwnAddress2(hw_i2c_handle);
  LL_I2C_DisableGeneralCall(hw_i2c_handle);
  LL_I2C_EnableClockStretching(hw_i2c_handle);

  if (LL_I2C_Init(hw_i2c_handle, config) == ERROR) {
    LOG_ERROR("I2C Init Failed!");
    error_handler();
  }

  LL_I2C_SetOwnAddress2(hw_i2c_handle, 0, LL_I2C_OWNADDRESS2_NOMASK);
  instance->handle = hw_i2c_handle;
}
