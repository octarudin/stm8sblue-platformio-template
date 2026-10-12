#include "platform/drivers/tim1_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_TIM1
#include "stm8s.h"

void tim1_driver_init(void)
{
    TIM1_DeInit();
    TIM1_TimeBaseInit(15U, TIM1_COUNTERMODE_UP, 999U, 0U);
    TIM1_Cmd(ENABLE);
}
#endif
