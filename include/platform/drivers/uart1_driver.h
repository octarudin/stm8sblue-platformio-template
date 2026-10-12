#ifndef PLATFORM_DRIVERS_UART1_DRIVER_H
#define PLATFORM_DRIVERS_UART1_DRIVER_H

#include <stdint.h>

void uart1_driver_init(uint32_t baud_rate);
void uart1_driver_write(const char *text);

#endif
