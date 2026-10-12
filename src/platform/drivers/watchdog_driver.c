#include "platform/drivers/watchdog_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_IWDG || STM8_FEATURE_WWDG
#include "stm8s.h"
#endif

#if STM8_FEATURE_IWDG
void iwdg_driver_init(void)
{
    IWDG_Enable();
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0xFFU);
    IWDG_ReloadCounter();
}
#endif

#if STM8_FEATURE_WWDG
void wwdg_driver_init(void)
{
    WWDG_Init(0x7FU, 0x7FU);
}
#endif
