#include "imu.h"

#include "board.h"
#include "common.h"
#include "i2c.h"
#include "logger.h"

#define PROCESS_PERIOD_MS    1000

typedef struct {
  uint32_t last_ticks;
  i2c_t    i2c_instance;
} state_t;

static state_t s;

static void hw_init(void)
{
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  LL_GPIO_InitTypeDef gpio_config = { 0 };
  gpio_config.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &gpio_config);

  LL_I2C_InitTypeDef i2c_config = { 0 };
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  i2c_config.PeripheralMode = LL_I2C_MODE_I2C;
  i2c_config.Timing = 0x00707CBB;
  i2c_config.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  i2c_config.DigitalFilter = 0;
  i2c_config.OwnAddress1 = 0;
  i2c_config.TypeAcknowledge = LL_I2C_ACK;
  i2c_config.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &i2c_config);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);

  i2c_init(&s.i2c_instance, I2C1, &i2c_config);
}

void imu_init(void)
{
  hw_init();
}

void imu_process(void)
{
  uint32_t time = system_time_get();

  if (system_time_cmp_ms(s.last_ticks, time) < PROCESS_PERIOD_MS) {
    return;
  }
  s.last_ticks = time;

  uint8_t read_val;
  i2c_read(&s.i2c_instance, 0x28 << 1, 0x0, &read_val, 1);

  LOG_INFO("i2c read: %d\r\n", read_val);


  LOG_DEBUG("imu process\r\n");
}
