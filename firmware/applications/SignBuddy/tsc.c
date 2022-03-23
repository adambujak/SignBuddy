#include "tsc.h"

#include "board.h"
#include "common.h"
#include "logger.h"

#include <string.h>

#define CALIBRATION_SAMPLES            8
#define TOUCH_TRIGGER_LEVEL            25

#define CHANNEL0                       0
#define CHANNEL1                       1
#define CHANNEL2                       2

#define TOUCH_SENSOR_0                 0   // TSC_GROUP2_CHANNEL0
#define TOUCH_SENSOR_1                 1   // TSC_GROUP4_CHANNEL0
#define TOUCH_SENSOR_2                 2   // TSC_GROUP5_CHANNEL0
#define TOUCH_SENSOR_3                 3   // TSC_GROUP6_CHANNEL0

#define TOUCH_SENSOR_4                 4   // TSC_GROUP2_CHANNEL1
#define TOUCH_SENSOR_5                 5   // TSC_GROUP4_CHANNEL1
#define TOUCH_SENSOR_6                 6   // TSC_GROUP5_CHANNEL1
#define TOUCH_SENSOR_7                 7   // TSC_GROUP6_CHANNEL1

#define TOUCH_SENSOR_8                 8   // TSC_GROUP2_CHANNEL2
#define TOUCH_SENSOR_9                 9   // TSC_GROUP4_CHANNEL2
#define TOUCH_SENSOR_10                10  // TSC_GROUP5_CHANNEL2
#define TOUCH_SENSOR_11                11  // TSC_GROUP6_CHANNEL2


#define SENSOR_PORT(group, channel)    TSC_GROUP ## group ## _CHANNEL ## channel ## _PORT
#define SENSOR_PIN(group, channel)     TSC_GROUP ## group ## _CHANNEL ## channel ## _PIN
#define SENSOR_IO(group, channel)      TSC_GROUP ## group ## _CHANNEL ## channel ## _IO

#define SAMPLER_PORT(group)            TSC_GROUP ## group ## _SAMPLER_PORT
#define SAMPLER_PIN(group)             TSC_GROUP ## group ## _SAMPLER_PIN
#define SAMPLER_IO(group)              TSC_GROUP ## group ## _SAMPLER_IO

#define CHANNEL0_IOS                   (SENSOR_IO(2, 0) | SENSOR_IO(4, 0) | SENSOR_IO(5, 0) | SENSOR_IO(6, 0))
#define CHANNEL1_IOS                   (SENSOR_IO(2, 1) | SENSOR_IO(4, 1) | SENSOR_IO(5, 1) | SENSOR_IO(6, 1))
#define CHANNEL2_IOS                   (SENSOR_IO(2, 2) | SENSOR_IO(4, 2) | SENSOR_IO(5, 2) | SENSOR_IO(6, 2))

#define SAMPLING0_IOS                  (SAMPLER_IO(2) | SAMPLER_IO(4) | SAMPLER_IO(5) | SAMPLER_IO(6))
#define SAMPLING1_IOS                  (SAMPLER_IO(2) | SAMPLER_IO(4) | SAMPLER_IO(5) | SAMPLER_IO(6))
#define SAMPLING2_IOS                  (SAMPLER_IO(2) | SAMPLER_IO(4) | SAMPLER_IO(5) | SAMPLER_IO(6))

#define CHANNELS_PER_GROUP             4
#define CHANNELS_CNT                   3

static const uint8_t CHANNEL0_GROUPS[] = { TSC_GROUP2_IDX, TSC_GROUP4_IDX, TSC_GROUP5_IDX, TSC_GROUP6_IDX };
static const uint8_t CHANNEL1_GROUPS[] = { TSC_GROUP2_IDX, TSC_GROUP4_IDX, TSC_GROUP5_IDX, TSC_GROUP6_IDX };
static const uint8_t CHANNEL2_GROUPS[] = { TSC_GROUP2_IDX, TSC_GROUP4_IDX, TSC_GROUP5_IDX, TSC_GROUP6_IDX };

static const uint8_t CHANNEL0_IDS[] = { TOUCH_SENSOR_0, TOUCH_SENSOR_1, TOUCH_SENSOR_2, TOUCH_SENSOR_3 };
static const uint8_t CHANNEL1_IDS[] = { TOUCH_SENSOR_4, TOUCH_SENSOR_5, TOUCH_SENSOR_6, TOUCH_SENSOR_7 };
static const uint8_t CHANNEL2_IDS[] = { TOUCH_SENSOR_8, TOUCH_SENSOR_9, TOUCH_SENSOR_10, TOUCH_SENSOR_11 };

static uint8_t const *const CHANNEL_IDS[CHANNELS_CNT] = { CHANNEL0_IDS, CHANNEL1_IDS, CHANNEL2_IDS };
static uint8_t const *const CHANNEL_GROUPS[CHANNELS_CNT] = { CHANNEL0_GROUPS, CHANNEL1_GROUPS, CHANNEL2_GROUPS };

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

  electrode_pin_init(SENSOR_PIN(2, 0), SENSOR_PORT(2, 0));
  electrode_pin_init(SENSOR_PIN(2, 1), SENSOR_PORT(2, 1));
  electrode_pin_init(SENSOR_PIN(2, 2), SENSOR_PORT(2, 2));
  electrode_pin_init(SENSOR_PIN(4, 0), SENSOR_PORT(4, 0));
  electrode_pin_init(SENSOR_PIN(4, 1), SENSOR_PORT(4, 1));
  electrode_pin_init(SENSOR_PIN(4, 2), SENSOR_PORT(4, 2));
  electrode_pin_init(SENSOR_PIN(5, 0), SENSOR_PORT(5, 0));
  electrode_pin_init(SENSOR_PIN(5, 1), SENSOR_PORT(5, 1));
  electrode_pin_init(SENSOR_PIN(5, 2), SENSOR_PORT(5, 2));
  electrode_pin_init(SENSOR_PIN(6, 0), SENSOR_PORT(6, 0));
  electrode_pin_init(SENSOR_PIN(6, 1), SENSOR_PORT(6, 1));
  electrode_pin_init(SENSOR_PIN(6, 2), SENSOR_PORT(6, 2));

  sampler_pin_init(SAMPLER_PIN(2), SAMPLER_PORT(2));
  sampler_pin_init(SAMPLER_PIN(4), SAMPLER_PORT(4));
  sampler_pin_init(SAMPLER_PIN(5), SAMPLER_PORT(5));
  sampler_pin_init(SAMPLER_PIN(6), SAMPLER_PORT(6));

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
  else if (channel == CHANNEL2) {
    io_config.ChannelIOs = CHANNEL2_IOS;
    io_config.SamplingIOs = SAMPLING2_IOS;
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

static inline int run_sampler(uint8_t channel)
{
  io_config(channel);
  HAL_TSC_IODischarge(&s.tsc, ENABLE);
  rtos_delay_ms(1);

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

static void sample_channel(uint8_t channel)
{
  if (run_sampler(channel) == RET_OK) {
    uint8_t const *groups = CHANNEL_GROUPS[channel];
    uint8_t const *ids = CHANNEL_IDS[channel];

    for (int i = 0; i < CHANNELS_PER_GROUP; i++) {
      uint32_t value;
      get_group_value(groups[i], &value);
      if (convert_value_to_touch(value, ids[i]) == 1) {
        s.touch_values |= (1 << ids[i]);
      }
    }
  }
  else {
    LOG_WARN("WARNING: TSC MAX CNT REACHED\r\n");
  }
}

static inline void sample_data(void)
{
  s.touch_values = 0;

  sample_channel(CHANNEL0);
  sample_channel(CHANNEL1);
  sample_channel(CHANNEL2);
}

static int calibrate_channel(uint8_t channel)
{
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    // TODO find bug
    // if log is added here, lower values are read (closer to what is seen during sampling)

    if (run_sampler(channel) == RET_OK) {
      uint8_t const *groups = CHANNEL_GROUPS[channel];
      uint8_t const *ids = CHANNEL_IDS[channel];

      for (int i = 0; i < CHANNELS_PER_GROUP; i++) {
        uint32_t value;
        get_group_value(groups[i], &value);
        s.calibration_values[ids[i]] += value / CALIBRATION_SAMPLES;
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

  if (calibrate_channel(CHANNEL0) != RET_OK) {
    return;
  }

  if (calibrate_channel(CHANNEL1) != RET_OK) {
    return;
  }

  if (calibrate_channel(CHANNEL2) != RET_OK) {
    return;
  }
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

void tsc_data_get(SBPSample_TouchData *data)
{
  data->touch_1 = (s.touch_values >> 0) & 0x1;
  data->touch_2 = (s.touch_values >> 1) & 0x1;
  data->touch_3 = (s.touch_values >> 2) & 0x1;
  data->touch_4 = (s.touch_values >> 3) & 0x1;
  data->touch_5 = (s.touch_values >> 4) & 0x1;
  data->touch_6 = (s.touch_values >> 5) & 0x1;
  data->touch_7 = (s.touch_values >> 6) & 0x1;
  data->touch_8 = (s.touch_values >> 7) & 0x1;
  data->touch_9 = (s.touch_values >> 8) & 0x1;
  data->touch_10 = (s.touch_values >> 9) & 0x1;
  data->touch_11 = (s.touch_values >> 10) & 0x1;
  data->touch_12 = (s.touch_values >> 11) & 0x1;
}

void tsc_callback_register(void (*callback)(void))
{
  s.callback = callback;
}
