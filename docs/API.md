# Referensi API

Header merupakan sumber deklarasi utama. Application sebaiknya hanya memakai
Application API dan Platform Façade API; API board/driver merupakan detail
internal platform.

## Entry point

### `void main(void)`

Lokasi: `src/main.c`

Membuat objek platform, memanggil `stm8_platform_init()` dan `app_init()`, lalu
menjalankan `app_run_once()` terus-menerus.

## Application API

### `void app_init(const stm8_platform_t *platform)`

Dipanggil satu kali setelah platform selesai diinisialisasi. Contoh bawaan
mengirim pesan startup melalui UART1.

### `void app_run_once(const stm8_platform_t *platform)`

Menjalankan satu iterasi use case. Contoh bawaan mengirim heartbeat, membalik
LED, lalu melakukan delay blocking 1000 ms. Kondisi LED berubah setiap satu
detik; satu periode ON/OFF lengkap berlangsung sekitar dua detik.

## Platform Façade API

Deklarasi: `include/platform/stm8_platform.h`  
Implementasi: `src/platform/stm8_platform.c`

### `void stm8_platform_init(const stm8_platform_t *platform)`

Menginisialisasi board dan driver yang lolos dua kondisi:

1. Source-nya diikutkan melalui `STM8_FEATURE_*`.
2. Bit periferal terdapat dalam `platform->enabled`.

Urutan default dimulai dari clock, dilanjutkan board/periferal, kemudian
watchdog. Jangan memanggil API platform lain sebelum fungsi ini.

### `uint8_t stm8_platform_is_enabled(...)`

```c
uint8_t stm8_platform_is_enabled(
    const stm8_platform_t *platform,
    stm8_peripheral_t peripheral
);
```

Mengembalikan `1` jika bit periferal aktif pada objek platform, atau `0` jika
tidak. Hasil `1` tidak berarti source driver tersedia apabila feature
compile-time-nya bernilai `0`.

### `void stm8_platform_uart1_write(...)`

```c
void stm8_platform_uart1_write(
    const stm8_platform_t *platform,
    const char *text
);
```

Mengirim string null-terminated melalui UART1 TX secara polling dan blocking.
Fungsi menjadi no-op ketika UART1 tidak dibangun atau tidak aktif.

### `void stm8_platform_delay_ms(...)`

```c
void stm8_platform_delay_ms(
    const stm8_platform_t *platform,
    uint16_t milliseconds
);
```

Menunggu secara blocking menggunakan TIM4. Konfigurasi default memberikan tick
nominal 1 ms pada HSI 16 MHz. Akurasi mengikuti clock aktual MCU. Jangan panggil
dari ISR.

### `void stm8_platform_led_toggle(...)`

```c
void stm8_platform_led_toggle(const stm8_platform_t *platform);
```

Membalik LED bawaan active-low di PB5. LED otomatis disiapkan oleh
`stm8_platform_init()`. Fungsi menjadi no-op ketika GPIO tidak dibangun atau
tidak aktif.

## Tipe dan macro platform

### `stm8_peripheral_t`

Bitmask seluruh periferal STM8S103F3P6 yang dikenal template. Beberapa nilai
dapat digabung dengan operator `|`.

### `stm8_platform_t`

```c
typedef struct {
    uint32_t enabled;
    uint32_t uart1_baud_rate;
    uint8_t tim4_period;
} stm8_platform_t;
```

- `enabled`: bitmask periferal aktif saat startup.
- `uart1_baud_rate`: baud rate UART1.
- `tim4_period`: auto-reload TIM4; default 124.

### `STM8_PLATFORM_ENABLED_MASK`

Membentuk bitmask otomatis dari seluruh `STM8_FEATURE_*`.

### `STM8_PLATFORM_DEFAULT`

Initializer platform dengan feature aktif, UART1 9600 baud, dan period TIM4
124.

## Board API internal

Deklarasi: `include/platform/board/stm8sblue_board.h`

| Fungsi | Deskripsi |
|---|---|
| `stm8sblue_board_init()` | Menginisialisasi LED PB5 sebagai push-pull, default mati |
| `stm8sblue_board_led_toggle()` | Membalik output LED PB5 |

Application tidak memanggil API ini secara langsung; gunakan façade platform.

## Driver API internal

Driver hanya dipanggil oleh façade platform atau modul platform lain.

| Modul | Fungsi | Deskripsi |
|---|---|---|
| ADC1 | `adc1_driver_init()` | Single conversion channel 0 |
| AWU | `awu_driver_init()` | Auto wake-up dengan timebase 1 detik |
| BEEP | `beep_driver_init()` | BEEP 1 kHz |
| Clock | `clock_driver_init()` | HSI tanpa pembagi, nominal 16 MHz |
| EXTI | `exti_driver_init()` | Mengembalikan EXTI ke konfigurasi awal |
| Flash | `flash_driver_init()` | Mengembalikan controller Flash ke konfigurasi awal |
| I2C | `i2c_driver_init()` | I2C default 100 kHz |
| SPI | `spi_driver_init()` | Master mode 0, MSB-first, CPU/16 |
| TIM1 | `tim1_driver_init()` | Time base default template |
| TIM2 | `tim2_driver_init()` | Time base default template |
| TIM4 | `tim4_delay_init()` | Menyiapkan tick delay |
| TIM4 | `tim4_delay_wait_ms()` | Delay blocking |
| UART1 | `uart1_driver_init()` | UART1 TX 8-N-1 |
| UART1 | `uart1_driver_write()` | TX string menggunakan polling |
| IWDG | `iwdg_driver_init()` | Mengaktifkan IWDG; tidak dapat dimatikan tanpa reset |
| WWDG | `wwdg_driver_init()` | Mengaktifkan WWDG; tidak dapat dimatikan tanpa reset |

Sebagian driver masih berupa startup scaffolding, bukan API operasional lengkap.
Periksa `docs/PERIPHERALS.md` sebelum menyatakan dukungan siap produksi.

## `assert_failed()`

Hanya diperlukan jika `USE_FULL_ASSERT` diaktifkan dalam `src/stm8s_conf.h`.
Template menonaktifkannya secara default untuk menghemat Flash.
