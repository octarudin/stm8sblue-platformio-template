#include "platform/drivers/tim2_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_TIM2
#include "stm8s.h"

void tim2_driver_init(void)
{
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 999U);
    TIM2_Cmd(ENABLE);
}
#endif
