#include "application/app.h"
#include "platform/stm8_platform.h"

static const stm8_platform_t platform = STM8_PLATFORM_DEFAULT;

void main(void)
{
    stm8_platform_init(&platform);
    app_init(&platform);

    while (1) {
        app_run_once(&platform);
    }
}
