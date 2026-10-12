#include "platform/drivers/clock_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_CLK
#include "stm8s.h"

void clock_driver_init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}
#endif
