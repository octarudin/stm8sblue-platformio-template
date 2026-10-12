#ifndef PLATFORM_DRIVERS_TIM4_DELAY_H
#define PLATFORM_DRIVERS_TIM4_DELAY_H

#include <stdint.h>

void tim4_delay_init(uint8_t period);
void tim4_delay_wait_ms(uint16_t milliseconds);

#endif
