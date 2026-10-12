# Periferal STM8S103F3P6

## Tingkat dukungan

`Defined` berarti periferal terdaftar pada objek platform dan memiliki feature flag. `Startup` berarti template memiliki inisialisasi default. `Operational API` berarti template menyediakan fungsi operasi yang dapat langsung dipanggil aplikasi.

| Periferal | Feature | Defined | Startup | Operational API | Catatan |
|---|---|---:|---:|---:|---|
| CLK | `STM8_FEATURE_CLK` | Ya | Ya | Tidak | Default HSI 16 MHz |
| GPIO | `STM8_FEATURE_GPIO` | Ya | Ya | Ya | Menyiapkan LED bawaan PB5; operasi lain ditambahkan sesuai kebutuhan |
| EXTI | `STM8_FEATURE_EXTI` | Ya | Sebagian | Tidak | Startup hanya reset konfigurasi EXTI |
| ADC1 | `STM8_FEATURE_ADC1` | Ya | Ya | Tidak | Default single conversion channel 0 |
| TIM1 | `STM8_FEATURE_TIM1` | Ya | Ya | Tidak | Counter default; PWM/capture belum dibungkus |
| TIM2 | `STM8_FEATURE_TIM2` | Ya | Ya | Tidak | Counter default; PWM/capture belum dibungkus |
| TIM4 | `STM8_FEATURE_TIM4` | Ya | Ya | Ya | Digunakan oleh delay blocking |
| UART1 | `STM8_FEATURE_UART1` | Ya | Ya | Ya | TX polling; RX belum dibungkus |
| I2C | `STM8_FEATURE_I2C` | Ya | Ya | Tidak | Default master-capable 100 kHz |
| SPI | `STM8_FEATURE_SPI` | Ya | Ya | Tidak | Default master mode 0, CPU/16 |
| AWU | `STM8_FEATURE_AWU` | Ya | Ya | Tidak | Default timebase 1 detik |
| BEEP | `STM8_FEATURE_BEEP` | Ya | Ya | Tidak | Default 1 kHz; pin/option byte perlu diverifikasi |
| FLASH | `STM8_FEATURE_FLASH` | Ya | Sebagian | Tidak | Startup melakukan deinit controller |
| ITC | `STM8_FEATURE_ITC` | Ya | Tidak | Tidak | Tidak memiliki global enable |
| RST | `STM8_FEATURE_RST` | Ya | Tidak | Tidak | Digunakan untuk membaca/bersihkan reset flags |
| IWDG | `STM8_FEATURE_IWDG` | Ya | Ya | Tidak | Tidak dapat dimatikan setelah aktif |
| WWDG | `STM8_FEATURE_WWDG` | Ya | Ya | Tidak | Tidak dapat dimatikan setelah aktif |

Jangan menyatakan sebuah periferal “siap produksi” hanya karena feature dapat dikompilasi. Driver operasional, validasi pin, error handling, timeout, dan pengujian hardware tetap diperlukan.

## Periferal yang tidak dimiliki STM8S103F3P6

Template tidak menyediakan feature untuk:

- ADC2
- UART2, UART3, UART4
- TIM3, TIM5, TIM6
- CAN

Header SPL mungkin tersedia karena package mendukung keluarga STM8 lain, tetapi periferal tersebut tidak tersedia pada target ini.

## Menambahkan API operasional

Ketika mulai menggunakan periferal, buat driver spesifik:

```text
include/platform/stm8/adc1_driver.h
src/platform/stm8/adc1_driver.c
```

Contoh API:

```c
void adc1_driver_init(void);
uint16_t adc1_driver_read(uint8_t channel);
```

Application sebaiknya tidak memanggil `ADC1_StartConversion()` secara langsung. Panggilan SPL tersebut menjadi detail implementasi driver platform.

## Watchdog

IWDG dan WWDG merupakan kasus khusus:

- Setelah diaktifkan, tidak dapat dinonaktifkan tanpa reset.
- Aplikasi wajib melakukan refresh dalam batas waktu.
- Mengaktifkan feature tanpa menambahkan refresh dapat menyebabkan reset berulang.
- Jangan mengaktifkannya hanya untuk menguji build.

## Konflik resource

- TIM4 digunakan oleh `stm8_platform_delay_ms()`.
- PB5 dimiliki modul board sebagai LED bawaan active-low.
- Jangan mengubah prescaler/period TIM4 dari modul lain tanpa memperbarui layanan waktu.
- UART1 memakai PD5/PD6.
- SPI, I2C, timer output, ADC, dan BEEP bergantung pada pin serta alternate function MCU.
- Dokumentasikan kepemilikan pin dan timer pada setiap proyek turunan.
