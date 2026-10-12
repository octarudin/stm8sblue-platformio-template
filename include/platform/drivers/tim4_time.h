#ifndef PLATFORM_DRIVERS_TIM4_TIME_H
#define PLATFORM_DRIVERS_TIM4_TIME_H

#include <stdint.h>

/** Menyiapkan TIM4 sebagai clock sistem periodik. */
void tim4_time_init(uint8_t period);

/** Mengaktifkan interrupt global setelah seluruh platform selesai diinisialisasi. */
void tim4_time_start(void);

/** Mengembalikan tick milidetik 16-bit; nilai wrap setiap 65,536 detik. */
uint16_t tim4_time_millis(void);

/** Delay blocking berbasis clock interrupt TIM4. */
void tim4_time_delay_ms(uint16_t milliseconds);

#endif
