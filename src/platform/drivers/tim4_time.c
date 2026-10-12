#include "platform/drivers/tim4_time.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_TIM4
#include "stm8s.h"

static volatile uint16_t system_time_ms;

void tim4_time_init(uint8_t period)
{
    system_time_ms = 0U;

    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, period);
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}

void tim4_time_start(void)
{
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    enableInterrupts();
}

uint16_t tim4_time_millis(void)
{
    uint16_t value;

    /* STM8 adalah CPU 8-bit; lindungi pembacaan counter 16-bit dari tearing. */
    disableInterrupts();
    value = system_time_ms;
    enableInterrupts();

    return value;
}

void tim4_time_delay_ms(uint16_t milliseconds)
{
    uint16_t started_at;

    started_at = tim4_time_millis();

    while ((uint16_t)(tim4_time_millis() - started_at) < milliseconds) {
    }
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    system_time_ms++;
}
#endif
