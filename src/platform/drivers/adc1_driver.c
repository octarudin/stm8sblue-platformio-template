#include "platform/drivers/adc1_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_ADC1
#include "stm8s.h"

void adc1_driver_init(void)
{
    ADC1_DeInit();
    ADC1_Init(
        ADC1_CONVERSIONMODE_SINGLE,
        ADC1_CHANNEL_0,
        ADC1_PRESSEL_FCPU_D18,
        ADC1_EXTTRIG_GPIO,
        DISABLE,
        ADC1_ALIGN_RIGHT,
        ADC1_SCHMITTTRIG_CHANNEL0,
        DISABLE
    );
    ADC1_Cmd(ENABLE);
}
#endif
