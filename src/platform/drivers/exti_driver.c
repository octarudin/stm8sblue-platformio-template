#include "platform/drivers/exti_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_EXTI
#include "stm8s.h"

void exti_driver_init(void)
{
    EXTI_DeInit();
}
#endif
