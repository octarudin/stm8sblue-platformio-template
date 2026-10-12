#include "platform/stm8_platform.h"
#include "platform/board/stm8sblue_board.h"
#include "platform/drivers/adc1_driver.h"
#include "platform/drivers/awu_driver.h"
#include "platform/drivers/beep_driver.h"
#include "platform/drivers/clock_driver.h"
#include "platform/drivers/exti_driver.h"
#include "platform/drivers/flash_driver.h"
#include "platform/drivers/i2c_driver.h"
#include "platform/drivers/spi_driver.h"
#include "platform/drivers/tim1_driver.h"
#include "platform/drivers/tim2_driver.h"
#include "platform/drivers/tim4_delay.h"
#include "platform/drivers/uart1_driver.h"
#include "platform/drivers/watchdog_driver.h"

uint8_t stm8_platform_is_enabled(
    const stm8_platform_t *platform,
    stm8_peripheral_t peripheral
)
{
    return (platform->enabled & (uint32_t)peripheral) != 0UL;
}

void stm8_platform_init(const stm8_platform_t *platform)
{
#if STM8_FEATURE_CLK
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_CLK)) {
        clock_driver_init();
    }
#endif

#if STM8_FEATURE_GPIO
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_GPIO)) {
        stm8sblue_board_init();
    }
#endif

#if STM8_FEATURE_EXTI
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_EXTI)) {
        exti_driver_init();
    }
#endif

#if STM8_FEATURE_FLASH
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_FLASH)) {
        flash_driver_init();
    }
#endif

#if STM8_FEATURE_ADC1
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_ADC1)) {
        adc1_driver_init();
    }
#endif

#if STM8_FEATURE_TIM1
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_TIM1)) {
        tim1_driver_init();
    }
#endif

#if STM8_FEATURE_TIM2
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_TIM2)) {
        tim2_driver_init();
    }
#endif

#if STM8_FEATURE_TIM4
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_TIM4)) {
        tim4_delay_init(platform->tim4_period);
    }
#endif

#if STM8_FEATURE_UART1
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_UART1)) {
        uart1_driver_init(platform->uart1_baud_rate);
    }
#endif

#if STM8_FEATURE_I2C
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_I2C)) {
        i2c_driver_init();
    }
#endif

#if STM8_FEATURE_SPI
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_SPI)) {
        spi_driver_init();
    }
#endif

#if STM8_FEATURE_AWU
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_AWU)) {
        awu_driver_init();
    }
#endif

#if STM8_FEATURE_BEEP
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_BEEP)) {
        beep_driver_init();
    }
#endif

    /* IWDG dan WWDG tidak dapat dimatikan lagi sebelum reset MCU. */
#if STM8_FEATURE_IWDG
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_IWDG)) {
        iwdg_driver_init();
    }
#endif

#if STM8_FEATURE_WWDG
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_WWDG)) {
        wwdg_driver_init();
    }
#endif

    /* ITC dan RST adalah layanan sistem; tidak memiliki global enable. */
    (void)platform;
}

void stm8_platform_uart1_write(const stm8_platform_t *platform, const char *text)
{
#if STM8_FEATURE_UART1
    if (!stm8_platform_is_enabled(platform, STM8_PERIPHERAL_UART1)) {
        return;
    }

    uart1_driver_write(text);
#else
    (void)platform;
    (void)text;
#endif
}

void stm8_platform_delay_ms(const stm8_platform_t *platform, uint16_t milliseconds)
{
#if STM8_FEATURE_TIM4
    if (!stm8_platform_is_enabled(platform, STM8_PERIPHERAL_TIM4)) {
        return;
    }

    tim4_delay_wait_ms(milliseconds);
#else
    (void)platform;
    (void)milliseconds;
#endif
}

void stm8_platform_led_toggle(const stm8_platform_t *platform)
{
#if STM8_FEATURE_GPIO
    if (stm8_platform_is_enabled(platform, STM8_PERIPHERAL_GPIO)) {
        stm8sblue_board_led_toggle();
    }
#else
    (void)platform;
#endif
}
