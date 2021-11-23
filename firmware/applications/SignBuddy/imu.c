#include "imu.h"

#include "board.h"
#include "common.h"
#include "i2c.h"
#include "logger.h"
#include "bno055.h"

#define PROCESS_PERIOD_MS    1000

typedef struct {
  uint32_t                last_ticks;
  i2c_t                   i2c_instance;
  struct   bno055_t       bno055;
  struct   bno055_accel_t bno055_accel_xyz;
  struct   bno055_mag_t   bno055_mag_xyz;
  struct   bno055_gyro_t  bno055_gyro_xyz;
} state_t;

static state_t s;

static void hw_init(void)
{
  IMU_I2C_CLK_EN();
  IMU_I2C_GPIO_CLK_EN();

  LL_GPIO_InitTypeDef gpio_config = { 0 };
  gpio_config.Pin = IMU_I2C_SCL_PIN | IMU_I2C_SDA_PIN;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = IMU_I2C_GPIO_AF;
  LL_GPIO_Init(IMU_I2C_GPIO_PORT, &gpio_config);

  LL_I2C_InitTypeDef i2c_config = { 0 };
  i2c_config.PeripheralMode = LL_I2C_MODE_I2C;
  i2c_config.Timing = 0x00300F38;
  i2c_config.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  i2c_config.DigitalFilter = 0;
  i2c_config.OwnAddress1 = 0;
  i2c_config.TypeAcknowledge = LL_I2C_ACK;
  i2c_config.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;

  i2c_init(&s.i2c_instance, IMU_I2C, &i2c_config);
}

static inline int8_t bus_write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
{
  i2c_write(&s.i2c_instance, slave_addr << 1, reg_addr, data, (uint16_t) length);
  return 0;
}

static inline int8_t bus_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
{
  i2c_read(&s.i2c_instance, slave_addr << 1, reg_addr, data, (uint16_t) length);
  return 0;
}

static inline void delay(u32 ms)
{
  delay_ms((uint32_t) ms);
}

static void bno_init(void)
{
  s.bno055.bus_write = bus_write;
  s.bno055.bus_read = bus_read;
  s.bno055.delay_msec = delay;
  s.bno055.dev_addr = BNO055_I2C_ADDR1;

  ERR_CHECK(bno055_init(&s.bno055));

  ERR_CHECK(bno055_set_power_mode(BNO055_POWER_MODE_NORMAL));

  delay_ms(1000);
  uint16_t sw_rev_id;
  ERR_CHECK(bno055_read_sw_rev_id(&sw_rev_id));

  uint8_t lsb = (uint8_t) sw_rev_id;
  uint8_t msb = (uint8_t) (sw_rev_id >> 8);

  LOG_INFO("IMU SW REV ID: %d %d.%d\r\n", sw_rev_id, msb, lsb);
  delay_ms(1000);
}

static void get_data(void)
{
  ERR_CHECK(bno055_set_operation_mode(BNO055_OPERATION_MODE_AMG));

  uint32_t ret = 0;
  ret |= bno055_read_accel_xyz(&s.bno055_accel_xyz);
  ret |= bno055_read_mag_xyz(&s.bno055_mag_xyz);
  ret |= bno055_read_gyro_xyz(&s.bno055_gyro_xyz);
  ERR_CHECK(ret);

  //LOG_INFO("Accel datax: %d\r\n", s.bno055_accel_xyz.x);
  //LOG_INFO("Accel datay: %d\r\n", s.bno055_accel_xyz.y);
  //LOG_INFO("Accel dataz: %d\r\n", s.bno055_accel_xyz.z);
  //LOG_INFO("Magnt datax: %d\r\n", s.bno055_mag_xyz.x);
  //LOG_INFO("Magnt datay: %d\r\n", s.bno055_mag_xyz.y);
  //LOG_INFO("Magnt dataz: %d\r\n", s.bno055_mag_xyz.z);
  //LOG_INFO("Gyros datax: %d\r\n", s.bno055_gyro_xyz.x);
  //LOG_INFO("Gyros datay: %d\r\n", s.bno055_gyro_xyz.y);
  //LOG_INFO("Gyros dataz: %d\r\n", s.bno055_gyro_xyz.z);
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

  get_data();
}
