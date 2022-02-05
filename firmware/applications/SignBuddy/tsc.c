#include "tsc.h"

#include "board.h"
#include "common.h"
#include "logger.h"

#define TOUCH_SENSOR_0                 TSC_GROUP3_CHANNEL0
#define TOUCH_SENSOR_1                 TSC_GROUP4_CHANNEL0
#define TOUCH_SENSOR_2                 TSC_GROUP4_CHANNEL1
#define TOUCH_SENSOR_3                 TSC_GROUP5_CHANNEL0
#define TOUCH_SENSOR_4                 TSC_GROUP5_CHANNEL1
#define TOUCH_SENSOR_5                 TSC_GROUP6_CHANNEL0
#define TOUCH_SENSOR_6                 TSC_GROUP6_CHANNEL1
#define TOUCH_SENSOR_7                 TSC_GROUP7_CHANNEL0
#define TOUCH_SENSOR_8                 TSC_GROUP7_CHANNEL1
#define TOUCH_SENSOR_9                 TSC_GROUP8_CHANNEL0
#define TOUCH_SENSOR_10                TSC_GROUP8_CHANNEL1

#define TOUCH_SENSOR_CNT               11

#define SENSOR_PORT(group, channel)    TSC_GROUP ## group ## _CHANNEL ## channel ## _PORT
#define SENSOR_PIN(group, channel)     TSC_GROUP ## group ## _CHANNEL ## channel ## _PIN

#define SAMPLER_PORT(group)            TSC_GROUP ## group ## _SAMPLER_PORT
#define SAMPLER_PIN(group)             TSC_GROUP ## group ## _SAMPLER_PIN

#define CALIBRATION_SAMPLES            8
#define TOUCH_TRIGGER_LEVEL            50

#define CHANNEL_IOS                    TSC_GROUP8_CHANNEL1_IO
#define SAMPLING_IOS                   TSC_GROUP8_SAMPLER_IO

typedef struct {
  TSC_HandleTypeDef tsc;
  // TODO add these
  // uint32_t electrode_measurement[TSC_ELECTRODE_CNT];
  uint32_t          calibration_value;
  int8_t            touch_value;
  TaskHandle_t      task_handle;
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
}

static void io_config(void)
{
  TSC_IOConfigTypeDef io_config;

  io_config.ChannelIOs = CHANNEL_IOS;
  io_config.SamplingIOs = SAMPLING_IOS;
  HAL_TSC_IOConfig(&s.tsc, &io_config);
}

static inline int convert_value_to_touch(uint32_t value)
{
  if (value < s.calibration_value) {
    if ((s.calibration_value - value) > TOUCH_TRIGGER_LEVEL) {
      return 1;
    }
  }
  return 0;
}

static int read_value(uint32_t *value)
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


  if (HAL_TSC_GroupGetStatus(&s.tsc, TSC_GROUP3_IDX) == TSC_GROUP_COMPLETED) {
    *value = HAL_TSC_GroupGetValue(&s.tsc, TSC_GROUP3_IDX);
  }
  return RET_OK;
}

static inline void sample_data(void)
{
  uint32_t value;

  if (read_value(&value) == RET_OK) {
    //LOG_DEBUG("tsc value: %lu cal: %lu\r\n", value, s.calibration_value);
    s.touch_value = convert_value_to_touch(value);
  }
  else {
    s.touch_value = 0;
  }

  io_config();
  HAL_TSC_IODischarge(&s.tsc, ENABLE);
}

static void calibrate(void)
{
  uint32_t calibration_value = 0;

  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    // TODO find bug
    // if log is added here, lower values are read (closer to what is seen during sampling)
    io_config();
    HAL_TSC_IODischarge(&s.tsc, ENABLE);

    rtos_delay_ms(10);

    uint32_t value;

    if (read_value(&value) == RET_OK) {
      calibration_value += value / CALIBRATION_SAMPLES;
    }
    else {
      // set to max value
      calibration_value = 0xFFFFFFFF;
    }
  }
  s.calibration_value = calibration_value;

  // discharge before we start sampling again
  io_config();
  HAL_TSC_IODischarge(&s.tsc, ENABLE);
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
  s.tsc.Init.ChannelIOs = TSC_GROUP8_CHANNEL1_IO;
  s.tsc.Init.ShieldIOs = 0;
  s.tsc.Init.SamplingIOs = TSC_GROUP8_SAMPLER_IO;

  if (HAL_TSC_Init(&s.tsc) != HAL_OK) {
    error_handler();
  }

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

void tsc_get_value(int8_t *measurement)
{
  *measurement = s.touch_value;
}

void tsc_callback_register(void (*callback)(void))
{
  s.callback = callback;
}
