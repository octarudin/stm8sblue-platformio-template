#include "platform/drivers/awu_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_AWU
#include "stm8s.h"

void awu_driver_init(void)
{
    AWU_DeInit();
    AWU_Init(AWU_TIMEBASE_1S);
    AWU_Cmd(ENABLE);
}
#endif
