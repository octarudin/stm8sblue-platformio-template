#include "platform/drivers/i2c_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_I2C
#include "stm8s.h"

void i2c_driver_init(void)
{
    I2C_DeInit();
    I2C_Init(
        100000UL,
        0U,
        I2C_DUTYCYCLE_2,
        I2C_ACK_CURR,
        I2C_ADDMODE_7BIT,
        16U
    );
    I2C_Cmd(ENABLE);
}
#endif
