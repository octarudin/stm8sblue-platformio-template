#include "platform/drivers/tim4_delay.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_TIM4
#include "stm8s.h"

void tim4_delay_init(uint8_t period)
{
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, period);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_Cmd(ENABLE);
}

void tim4_delay_wait_ms(uint16_t milliseconds)
{
    TIM4_SetCounter(0U);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);

    while (milliseconds > 0U) {
        while (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == RESET) {
        }
        TIM4_ClearFlag(TIM4_FLAG_UPDATE);
        milliseconds--;
    }
}
#endif
