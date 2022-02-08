#include "tsc.h"

#include "board.h"
#include "common.h"
#include "logger.h"

#include <string.h>

#define CALIBRATION_SAMPLES            8
#define TOUCH_TRIGGER_LEVEL            50

#define CHANNEL0                       0
#define CHANNEL1                       1

#define TOUCH_SENSOR_0                 0   // TSC_GROUP3_CHANNEL0
#define TOUCH_SENSOR_1                 1   // TSC_GROUP4_CHANNEL0
#define TOUCH_SENSOR_2                 2   // TSC_GROUP5_CHANNEL0
#define TOUCH_SENSOR_3                 3   // TSC_GROUP6_CHANNEL0
#define TOUCH_SENSOR_4                 4   // TSC_GROUP7_CHANNEL0
#define TOUCH_SENSOR_5                 5   // TSC_GROUP8_CHANNEL0

#define TOUCH_SENSOR_6                 6   // TSC_GROUP4_CHANNEL1
#define TOUCH_SENSOR_7                 7   // TSC_GROUP5_CHANNEL1
#define TOUCH_SENSOR_8                 8   // TSC_GROUP6_CHANNEL1
#define TOUCH_SENSOR_9                 9   // TSC_GROUP7_CHANNEL1
#define TOUCH_SENSOR_10                10  // TSC_GROUP8_CHANNEL1


#define SENSOR_PORT(group, channel)    TSC_GROUP ## group ## _CHANNEL ## channel ## _PORT
#define SENSOR_PIN(group, channel)     TSC_GROUP ## group ## _CHANNEL ## channel ## _PIN
#define SENSOR_IO(group, channel)      TSC_GROUP ## group ## _CHANNEL ## channel ## _IO

#define SAMPLER_PORT(group)            TSC_GROUP ## group ## _SAMPLER_PORT
#define SAMPLER_PIN(group)             TSC_GROUP ## group ## _SAMPLER_PIN
#define SAMPLER_IO(group)              TSC_GROUP ## group ## _SAMPLER_IO

// TODO when all sensors are available uncomment this and change numbers to enum
// since numbers are a source of a bug

/*
 #define TOUCH_SENSOR_CNT               11
 * // group info about all sensors on IO = 0
 #define CHANNEL0_MIN_GROUP             3
 #define CHANNEL0_MAX_GROUP             8
 #define CHANNEL0_CNT                   6
 *
 * // group info about all sensors on IO = 1
 #define CHANNEL1_MIN_GROUP             4
 #define CHANNEL1_MAX_GROUP             8
 #define CHANNEL1_CNT                   5
 *
 *
 #define CHANNEL0_IOS                   (SENSOR_IO(3, 0) | SENSOR_IO(4, 0) | SENSOR_IO(5, 0) | \
 *                                      SENSOR_IO(6, 0) | SENSOR_IO(7, 0) | SENSOR_IO(8, 0))
 *
 *
 #define CHANNEL1_IOS                   (SENSOR_IO(4, 1) | SENSOR_IO(5, 1) | SENSOR_IO(6, 1) | \
 *                                      SENSOR_IO(7, 1) | SENSOR_IO(8, 1))
 *
 #define SAMPLING0_IOS                  (SAMPLER_IO(3) | SAMPLER_IO(4) | SAMPLER_IO(5) | \
 *                                      SAMPLER_IO(6) | SAMPLER_IO(7) | SAMPLER_IO(8))
 *
 #define SAMPLING1_IOS                  (SAMPLER_IO(4) | SAMPLER_IO(5) | SAMPLER_IO(6) | \
 *                                      SAMPLER_IO(7) | SAMPLER_IO(8))
 */
// group info about all sensors on IO = 0
#define CHANNEL0_MIN_GROUP    TSC_GROUP3_IDX
#define CHANNEL0_MAX_GROUP    TSC_GROUP4_IDX
#define CHANNEL0_CNT          ((CHANNEL0_MAX_GROUP - CHANNEL0_MIN_GROUP) + 1)

// group info about all sensors on IO = 1
#define CHANNEL1_MIN_GROUP    TSC_GROUP4_IDX
#define CHANNEL1_MAX_GROUP    TSC_GROUP4_IDX
#define CHANNEL1_CNT          ((CHANNEL1_MAX_GROUP - CHANNEL1_MIN_GROUP) + 1)

#define CHANNEL0_IOS          (SENSOR_IO(3, 0) | SENSOR_IO(4, 0))

#define CHANNEL1_IOS          (SENSOR_IO(4, 1))

#define SAMPLING0_IOS         (SAMPLER_IO(3) | SAMPLER_IO(4))

#define SAMPLING1_IOS         (SAMPLER_IO(4))


typedef struct {
  TSC_HandleTypeDef tsc;
  TaskHandle_t      task_handle;
  uint16_t          touch_values;
  uint32_t          calibration_values[TOUCH_SENSOR_CNT];
  void (*callback)(void);
} state_t;

static state_t s;

static inline void sampler_pin_init(uint32_t pin, GPIO_TypeDef *port)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };

  gpio_config.Pin = pin;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  gpio_config.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = TSC_SAMPLER_AF;
  LL_GPIO_Init(port, &gpio_config);
}

static inline void electrode_pin_init(uint32_t pin, GPIO_TypeDef *port)
{
  LL_GPIO_InitTypeDef gpio_config = { 0 };

  gpio_config.Pin = pin;
  gpio_config.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_config.Speed = LL_GPIO_SPEED_FREQ_LOW;
  gpio_config.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.Pull = LL_GPIO_PULL_NO;
  gpio_config.Alternate = TSC_ELECTRODE_AF;
  LL_GPIO_Init(port, &gpio_config);
}

static void hw_init(void)
{
  TSC_CLK_EN();
  TSC_GPIO_CLK_EN();

  electrode_pin_init(SENSOR_PIN(3, 0), SENSOR_PORT(3, 0));
  electrode_pin_init(SENSOR_PIN(4, 0), SENSOR_PORT(4, 0));
  electrode_pin_init(SENSOR_PIN(4, 1), SENSOR_PORT(4, 1));
  electrode_pin_init(SENSOR_PIN(5, 0), SENSOR_PORT(5, 0));
  electrode_pin_init(SENSOR_PIN(5, 1), SENSOR_PORT(5, 1));
  electrode_pin_init(SENSOR_PIN(6, 0), SENSOR_PORT(6, 0));
  electrode_pin_init(SENSOR_PIN(6, 1), SENSOR_PORT(6, 1));
  electrode_pin_init(SENSOR_PIN(7, 0), SENSOR_PORT(7, 0));
  electrode_pin_init(SENSOR_PIN(7, 1), SENSOR_PORT(7, 1));
  electrode_pin_init(SENSOR_PIN(8, 0), SENSOR_PORT(8, 0));
  electrode_pin_init(SENSOR_PIN(8, 1), SENSOR_PORT(8, 1));

  sampler_pin_init(SAMPLER_PIN(3), SAMPLER_PORT(3));
  sampler_pin_init(SAMPLER_PIN(4), SAMPLER_PORT(4));
  sampler_pin_init(SAMPLER_PIN(5), SAMPLER_PORT(5));
  sampler_pin_init(SAMPLER_PIN(6), SAMPLER_PORT(6));
  sampler_pin_init(SAMPLER_PIN(7), SAMPLER_PORT(7));
  sampler_pin_init(SAMPLER_PIN(8), SAMPLER_PORT(8));

  s.tsc.Instance = TSC;
  s.tsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  s.tsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  s.tsc.Init.SpreadSpectrum = DISABLE;
  s.tsc.Init.SpreadSpectrumDeviation = 1;
  s.tsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  s.tsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  s.tsc.Init.MaxCountValue = TSC_MCV_8191;
  s.tsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  s.tsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  s.tsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  s.tsc.Init.MaxCountInterrupt = DISABLE;
  s.tsc.Init.ChannelIOs = 0;
  s.tsc.Init.ShieldIOs = 0;
  s.tsc.Init.SamplingIOs = 0;

  if (HAL_TSC_Init(&s.tsc) != HAL_OK) {
    error_handler();
  }
}

static inline void io_config(uint8_t channel)
{
  TSC_IOConfigTypeDef io_config = { 0 };

  if (channel == CHANNEL0) {
    io_config.ChannelIOs = CHANNEL0_IOS;
    io_config.SamplingIOs = SAMPLING0_IOS;
  }
  else if (channel == CHANNEL1) {
    io_config.ChannelIOs = CHANNEL1_IOS;
    io_config.SamplingIOs = SAMPLING1_IOS;
  }

  HAL_TSC_IOConfig(&s.tsc, &io_config);
}

static inline int convert_value_to_touch(uint32_t value, int index)
{
  if (value < s.calibration_values[index]) {
    if ((s.calibration_values[index] - value) > TOUCH_TRIGGER_LEVEL) {
      return 1;
    }
  }
  return 0;
}

static inline void get_group_value(uint8_t group, uint32_t *value)
{
  if (HAL_TSC_GroupGetStatus(&s.tsc, group) == TSC_GROUP_COMPLETED) {
    *value = HAL_TSC_GroupGetValue(&s.tsc, group);
  }
}

static inline int run_sampler(void)
{
  if (HAL_TSC_Start(&s.tsc) != HAL_OK) {
    error_handler();
  }

  while (HAL_TSC_GetState(&s.tsc) == HAL_TSC_STATE_BUSY);

  // if max cnt err occured return fail
  uint32_t isr_val = TSC->ISR;

  if (isr_val & TSC_ISR_MCEF) {
    return RET_ERR;
  }

  __HAL_TSC_CLEAR_FLAG(&s.tsc, (TSC_FLAG_EOA | TSC_FLAG_MCE));
  return RET_OK;
}

// offset is the offset in the touch value array for the channel
static void fetch_channel_values(uint8_t channel, uint8_t min_group, uint8_t max_group, uint8_t offset)
{
  io_config(channel);
  HAL_TSC_IODischarge(&s.tsc, ENABLE);
  rtos_delay_ms(1);

  int index = offset;
  uint32_t value;
  if (run_sampler() == RET_OK) {
    for (int i = min_group; i <= max_group; i++) {
      get_group_value(i, &value);
      if (convert_value_to_touch(value, index) == 1) {
        s.touch_values |= (1 << index);
      }
      index++;
    }
  }
  else {
    LOG_WARN("WARNING: TSC MAX CNT REACHED\r\n");
  }
}

static inline void sample_data(void)
{
  s.touch_values = 0;
  fetch_channel_values(CHANNEL0, CHANNEL0_MIN_GROUP, CHANNEL0_MAX_GROUP, 0);
  fetch_channel_values(CHANNEL1, CHANNEL1_MIN_GROUP, CHANNEL1_MAX_GROUP, CHANNEL0_CNT);
}

static int calibrate_channel(uint8_t channel, uint8_t min_group, uint8_t max_group, uint8_t offset)
{
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    // TODO find bug
    // if log is added here, lower values are read (closer to what is seen during sampling)
    io_config(channel);
    HAL_TSC_IODischarge(&s.tsc, ENABLE);
    rtos_delay_ms(10);

    if (run_sampler() == RET_OK) {
      uint32_t value;
      int index = offset;

      for (int i = min_group; i <= max_group; i++) {
        get_group_value(i, &value);
        s.calibration_values[index] += value / CALIBRATION_SAMPLES;
        index++;
      }
    }
    else {
      LOG_WARN("WARNING: TSC MAX CNT REACHED\r\n");
      // set to max value
      memset(s.calibration_values, 0xFF, sizeof(s.calibration_values));
      return RET_ERR;
    }
  }
  return RET_OK;
}

static void calibrate(void)
{
  memset(s.calibration_values, 0, sizeof(s.calibration_values));

  if (calibrate_channel(CHANNEL0, CHANNEL0_MIN_GROUP, CHANNEL0_MAX_GROUP, 0) != RET_OK) {
    return;
  }
  calibrate_channel(CHANNEL1, CHANNEL1_MIN_GROUP, CHANNEL1_MAX_GROUP, CHANNEL0_CNT);
}

static void tsc_task(void *arg)
{
  LOG_INFO("tsc: task started\r\n");
  while (1) {
    // wait to be notified
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    sample_data();
    s.callback();
  }
}

void tsc_task_setup(void)
{
  hw_init();

  calibrate();
}

void tsc_task_start(void)
{
  BaseType_t task_status = xTaskCreate(tsc_task, "tsc", TSC_STACK_SIZE, NULL, TSC_TASK_PRIORITY, &s.task_handle);

  RTOS_ERR_CHECK(task_status);
}

void tsc_start_read(void)
{
  xTaskNotifyGive(s.task_handle);
}

void tsc_data_get(Sample_TouchData *data)
{
  data->touch_values = (uint32_t) s.touch_values;
}

void tsc_callback_register(void (*callback)(void))
{
  s.callback = callback;
}
