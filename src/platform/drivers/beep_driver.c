#include "platform/drivers/beep_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_BEEP
#include "stm8s.h"

void beep_driver_init(void)
{
    BEEP_DeInit();
    BEEP_Init(BEEP_FREQUENCY_1KHZ);
    BEEP_Cmd(ENABLE);
}
#endif
