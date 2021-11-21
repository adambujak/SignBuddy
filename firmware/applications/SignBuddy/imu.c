#include "imu.h"

#include "board.h"
#include "common.h"
#include "i2c.h"
#include "logger.h"
#include "bno055.h"

#define PROCESS_PERIOD_MS    1000

struct bno055_t bno055;
struct bno055_accel_t bno055_accel_xyz;
struct bno055_mag_t bno055_mag_xyz;
struct bno055_gyro_t bno055_gyro_xyz;

typedef struct {
  uint32_t last_ticks;
  i2c_t    i2c_instance;
} state_t;

static state_t s;

static void hw_init(void)
{
  IMU_I2C_CLK_EN();
  IMU_I2C_GPIO_CLK_EN();

  LL_GPIO_InitTypeDef gpio_config = { 0 };
  gpio_config.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOB, &gpio_config);

  LL_I2C_InitTypeDef i2c_config = { 0 };
  LL_I2C_EnableAutoEndMode(IMU_I2C);
  LL_I2C_DisableOwnAddress2(IMU_I2C);
  LL_I2C_DisableGeneralCall(IMU_I2C);
  LL_I2C_EnableClockStretching(IMU_I2C);
  i2c_config.PeripheralMode = LL_I2C_MODE_I2C;
  i2c_config.Timing = 0x00707CBB;
  i2c_config.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  i2c_config.DigitalFilter = 0;
  i2c_config.OwnAddress1 = 0;
  i2c_config.TypeAcknowledge = LL_I2C_ACK;
  i2c_config.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_SetOwnAddress2(IMU_I2C, 0, LL_I2C_OWNADDRESS2_NOMASK);

  i2c_init(&s.i2c_instance, IMU_I2C, &i2c_config);
}

static inline int8_t write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
  i2c_write(&s.i2c_instance, dev_addr << 1, reg_addr, reg_data, (uint16_t) cnt);
  return 0;
}

static inline int8_t read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
  i2c_read(&s.i2c_instance, dev_addr << 1, reg_addr, reg_data, (uint16_t) cnt);
  return 0;
}

static inline void delay(u32 ms)
{
  delay_ms((uint32_t) ms);
}

static void bno_init(void)
{
  bno055.bus_write = write;
  bno055.bus_read = read;
  bno055.delay_msec = delay;
  bno055.dev_addr = BNO055_I2C_ADDR1;

  ERR_CHECK(bno055_init(&bno055));

  ERR_CHECK(bno055_set_power_mode(BNO055_POWER_MODE_NORMAL));
}

static void get_data(void)
{
  ERR_CHECK(bno055_set_operation_mode(BNO055_OPERATION_MODE_AMG));

  uint32_t ret = 0;
  ret |= bno055_read_accel_xyz(&bno055_accel_xyz);
  ret |= bno055_read_mag_xyz(&bno055_mag_xyz);
  ret |= bno055_read_gyro_xyz(&bno055_gyro_xyz);
  ERR_CHECK(ret);

  LOG_INFO("Accel datax: %d\r\n", bno055_accel_xyz.x);
  LOG_INFO("Accel datay: %d\r\n", bno055_accel_xyz.y);
  LOG_INFO("Accel dataz: %d\r\n", bno055_accel_xyz.z);
  LOG_INFO("Mag datax: %d\r\n", bno055_mag_xyz.x);
  LOG_INFO("Mag datay: %d\r\n", bno055_mag_xyz.y);
  LOG_INFO("Mag dataz: %d\r\n", bno055_mag_xyz.z);
  LOG_INFO("Gyro datax: %d\r\n", bno055_gyro_xyz.x);
  LOG_INFO("Gyro datay: %d\r\n", bno055_gyro_xyz.y);
  LOG_INFO("Gyro dataz: %d\r\n", bno055_gyro_xyz.z);
}

void imu_init(void)
{
  hw_init();
  bno_init();
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

  get_data();
}
