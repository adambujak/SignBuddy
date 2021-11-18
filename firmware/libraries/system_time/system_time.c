#include "system_time.h"

#include "board.h"
#include "common.h"

#define TIMER_FREQ            (1000000)
#define PRESCALER             (((SYSCLK_FREQ) / TIMER_FREQ) - 1)
#define AUTORELOAD            0xFFFF

#define ST_TICKS_TO_US(ticks)    (ticks)
#define ST_TICKS_TO_MS(ticks)    ((ticks) / 1000)

typedef struct {
  uint32_t overrun_cnt;
} state_t;

static state_t s;

static void timer_init(void)
{
  SYSTEM_TIME_CLK_EN();

  LL_TIM_InitTypeDef timer_config = { 0 };

  timer_config.Prescaler = PRESCALER;
  timer_config.CounterMode = LL_TIM_COUNTERMODE_UP;
  timer_config.Autoreload = AUTORELOAD;
  timer_config.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;

  LL_TIM_Init(SYSTEM_TIME_TIMER, &timer_config);
  LL_TIM_EnableARRPreload(SYSTEM_TIME_TIMER);
  LL_TIM_SetClockSource(SYSTEM_TIME_TIMER, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(SYSTEM_TIME_TIMER, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(SYSTEM_TIME_TIMER);
  LL_TIM_EnableCounter(SYSTEM_TIME_TIMER);

  LL_TIM_EnableUpdateEvent(SYSTEM_TIME_TIMER);
  LL_TIM_EnableIT_UPDATE(SYSTEM_TIME_TIMER);

  NVIC_SetPriority(SYSTEM_TIME_TIMER_IRQn, SYSTEM_TIME_PRIORITY);
  NVIC_EnableIRQ(SYSTEM_TIME_TIMER_IRQn);
}

inline uint32_t system_time_cmp_ticks(uint32_t old_time, uint32_t new_time)
{
  if (new_time < old_time) {
    return (AUTORELOAD - old_time) + new_time;
  }
  return new_time - old_time;
}

inline uint32_t system_time_cmp_us(uint32_t old_time, uint32_t new_time)
{
  uint32_t tick_diff = system_time_cmp_ticks(old_time, new_time);

  return ST_TICKS_TO_US(tick_diff);
}

inline uint32_t system_time_cmp_ms(uint32_t old_time, uint32_t new_time)
{
  uint32_t tick_diff = system_time_cmp_ticks(old_time, new_time);

  return ST_TICKS_TO_MS(tick_diff);
}

uint32_t system_time_get(void)
{
  DISABLE_IRQ();
  uint32_t time = (AUTORELOAD * s.overrun_cnt) + SYSTEM_TIME_TIMER->CNT;
  ENABLE_IRQ();
  return time;
}

void system_time_init(void)
{
  timer_init();
}

void SYSTEM_TIME_TIMER_IRQHandler(void)
{
  DISABLE_IRQ();
  LL_TIM_ClearFlag_UPDATE(SYSTEM_TIME_TIMER);
  s.overrun_cnt++;
  ENABLE_IRQ();
}
