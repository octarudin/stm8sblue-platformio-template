#include "application/app.h"

void app_init(const stm8_platform_t *platform)
{
    stm8_platform_uart1_write(platform, "STM8S template ready\r\n");
}

void app_run_once(const stm8_platform_t *platform)
{
    stm8_platform_uart1_write(platform, "Application heartbeat\r\n");
    stm8_platform_led_toggle(platform);
    stm8_platform_delay_ms(platform, 1000U);
}
