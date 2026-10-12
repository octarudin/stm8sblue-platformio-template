#ifndef PLATFORM_FEATURES_H
#define PLATFORM_FEATURES_H

/*
 * Sakelar compile-time periferal STM8S103F3P6.
 *
 * Ubah 0 menjadi 1 untuk memasukkan driver SPL ke firmware. Periferal yang
 * bernilai 0 tetap dikenal oleh objek platform, tetapi implementasi SPL-nya
 * tidak ikut di-link agar Flash 8 KB tidak cepat habis.
 */
#define STM8_FEATURE_ADC1   0
#define STM8_FEATURE_AWU    0
#define STM8_FEATURE_BEEP   0
#define STM8_FEATURE_CLK    1
#define STM8_FEATURE_EXTI   0
#define STM8_FEATURE_FLASH  0
#define STM8_FEATURE_GPIO   1
#define STM8_FEATURE_I2C    0
#define STM8_FEATURE_ITC    0
#define STM8_FEATURE_IWDG   0
#define STM8_FEATURE_RST    0
#define STM8_FEATURE_SPI    0
#define STM8_FEATURE_TIM1   0
#define STM8_FEATURE_TIM2   0
#define STM8_FEATURE_TIM4   1
#define STM8_FEATURE_UART1  1
#define STM8_FEATURE_WWDG   0

#endif
