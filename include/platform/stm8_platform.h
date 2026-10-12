#ifndef STM8_PLATFORM_H
#define STM8_PLATFORM_H

#include <stdint.h>
#include "platform_features.h"

/* Semua periferal yang tersedia pada STM8S103F3P6. */
typedef enum {
    STM8_PERIPHERAL_ADC1  = (1UL << 0),
    STM8_PERIPHERAL_AWU   = (1UL << 1),
    STM8_PERIPHERAL_BEEP  = (1UL << 2),
    STM8_PERIPHERAL_CLK   = (1UL << 3),
    STM8_PERIPHERAL_EXTI  = (1UL << 4),
    STM8_PERIPHERAL_FLASH = (1UL << 5),
    STM8_PERIPHERAL_GPIO  = (1UL << 6),
    STM8_PERIPHERAL_I2C   = (1UL << 7),
    STM8_PERIPHERAL_ITC   = (1UL << 8),
    STM8_PERIPHERAL_IWDG  = (1UL << 9),
    STM8_PERIPHERAL_RST   = (1UL << 10),
    STM8_PERIPHERAL_SPI   = (1UL << 11),
    STM8_PERIPHERAL_TIM1  = (1UL << 12),
    STM8_PERIPHERAL_TIM2  = (1UL << 13),
    STM8_PERIPHERAL_TIM4  = (1UL << 14),
    STM8_PERIPHERAL_UART1 = (1UL << 15),
    STM8_PERIPHERAL_WWDG  = (1UL << 16)
} stm8_peripheral_t;

typedef struct {
    /** Bitmask stm8_peripheral_t yang dibangun dan diaktifkan saat startup. */
    uint32_t enabled;
    /** Baud rate UART1 ketika feature UART1 aktif. */
    uint32_t uart1_baud_rate;
    /** Nilai auto-reload TIM4; 124 menghasilkan tick 1 ms pada 16 MHz / 128. */
    uint8_t tim4_period;
} stm8_platform_t;

#define STM8_PLATFORM_ENABLED_MASK ( \
    (STM8_FEATURE_ADC1  ? STM8_PERIPHERAL_ADC1  : 0UL) | \
    (STM8_FEATURE_AWU   ? STM8_PERIPHERAL_AWU   : 0UL) | \
    (STM8_FEATURE_BEEP  ? STM8_PERIPHERAL_BEEP  : 0UL) | \
    (STM8_FEATURE_CLK   ? STM8_PERIPHERAL_CLK   : 0UL) | \
    (STM8_FEATURE_EXTI  ? STM8_PERIPHERAL_EXTI  : 0UL) | \
    (STM8_FEATURE_FLASH ? STM8_PERIPHERAL_FLASH : 0UL) | \
    (STM8_FEATURE_GPIO  ? STM8_PERIPHERAL_GPIO  : 0UL) | \
    (STM8_FEATURE_I2C   ? STM8_PERIPHERAL_I2C   : 0UL) | \
    (STM8_FEATURE_ITC   ? STM8_PERIPHERAL_ITC   : 0UL) | \
    (STM8_FEATURE_IWDG  ? STM8_PERIPHERAL_IWDG  : 0UL) | \
    (STM8_FEATURE_RST   ? STM8_PERIPHERAL_RST   : 0UL) | \
    (STM8_FEATURE_SPI   ? STM8_PERIPHERAL_SPI   : 0UL) | \
    (STM8_FEATURE_TIM1  ? STM8_PERIPHERAL_TIM1  : 0UL) | \
    (STM8_FEATURE_TIM2  ? STM8_PERIPHERAL_TIM2  : 0UL) | \
    (STM8_FEATURE_TIM4  ? STM8_PERIPHERAL_TIM4  : 0UL) | \
    (STM8_FEATURE_UART1 ? STM8_PERIPHERAL_UART1 : 0UL) | \
    (STM8_FEATURE_WWDG  ? STM8_PERIPHERAL_WWDG  : 0UL))

/* Satu objek konfigurasi: HSI 16 MHz, UART 9600, tick TIM4 1 ms. */
#define STM8_PLATFORM_DEFAULT { STM8_PLATFORM_ENABLED_MASK, 9600UL, 124U }

/**
 * Menginisialisasi seluruh periferal yang aktif dalam konfigurasi platform.
 *
 * Harus dipanggil satu kali sebelum API platform lainnya. Pointer tidak boleh
 * NULL. Konfigurasi hardware default untuk setiap feature berada dalam
 * implementasi platform.
 */
void stm8_platform_init(const stm8_platform_t *platform);

/**
 * Memeriksa apakah satu periferal aktif pada objek platform.
 *
 * @return 1 jika aktif, 0 jika tidak aktif.
 */
uint8_t stm8_platform_is_enabled(
    const stm8_platform_t *platform,
    stm8_peripheral_t peripheral
);

/**
 * Mengirim string null-terminated secara blocking melalui UART1.
 *
 * Fungsi menjadi no-op jika UART1 tidak dibangun atau tidak aktif.
 */
void stm8_platform_uart1_write(const stm8_platform_t *platform, const char *text);

/**
 * Menunggu sejumlah milidetik secara blocking menggunakan TIM4.
 *
 * Fungsi menjadi no-op jika TIM4 tidak dibangun atau tidak aktif. Akurasi
 * mengikuti akurasi sumber clock MCU.
 */
void stm8_platform_delay_ms(const stm8_platform_t *platform, uint16_t milliseconds);

/**
 * Mengembalikan clock sistem dalam milidetik dari TIM4.
 *
 * Nilai 16-bit wrap setiap 65,536 detik. Fungsi mengembalikan 0 jika TIM4
 * tidak dibangun atau tidak aktif.
 */
uint16_t stm8_platform_millis(const stm8_platform_t *platform);

/**
 * Membalik kondisi LED bawaan STM8SBlue.
 *
 * Fungsi menjadi no-op jika GPIO tidak dibangun atau tidak aktif. LED
 * diinisialisasi otomatis oleh stm8_platform_init().
 */
void stm8_platform_led_toggle(const stm8_platform_t *platform);

#endif
