#include "application/app.h"
#include "kernel/cooperative_scheduler.h"

#define LED_TASK_PERIOD_MS       1000U
#define HEARTBEAT_TASK_PERIOD_MS 1000U

static const stm8_platform_t *active_platform;

static void led_task(void)
{
    stm8_platform_led_toggle(active_platform);
}

static void heartbeat_task(void)
{
    stm8_platform_uart1_write(active_platform, "Application heartbeat\r\n");
}

void app_init(const stm8_platform_t *platform)
{
    scheduler_task_id_t led_task_id;
    scheduler_task_id_t heartbeat_task_id;

    active_platform = platform;
    scheduler_init(stm8_platform_millis(platform));

    led_task_id = scheduler_add(
        led_task,
        LED_TASK_PERIOD_MS,
        LED_TASK_PERIOD_MS
    );
    heartbeat_task_id = scheduler_add(
        heartbeat_task,
        HEARTBEAT_TASK_PERIOD_MS,
        HEARTBEAT_TASK_PERIOD_MS
    );

    stm8_platform_uart1_write(platform, "STM8S template ready\r\n");

    if ((led_task_id == SCHEDULER_INVALID_TASK_ID) ||
        (heartbeat_task_id == SCHEDULER_INVALID_TASK_ID)) {
        stm8_platform_uart1_write(platform, "Scheduler registration failed\r\n");
    }
}

void app_run_once(const stm8_platform_t *platform)
{
    scheduler_run(stm8_platform_millis(platform));
}
