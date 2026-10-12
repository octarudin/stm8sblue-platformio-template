#include "platform/drivers/flash_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_FLASH
#include "stm8s.h"

void flash_driver_init(void)
{
    FLASH_DeInit();
}
#endif
