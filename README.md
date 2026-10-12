# STM8S Blue PlatformIO Template

Template firmware untuk board STM8S103F3P6 (`stm8sblue`) menggunakan PlatformIO, Standard Peripheral Library (SPL), dan lightweight Clean Architecture.

Template ini menyediakan:

- Build dan upload melalui ST-LINK V2.
- Objek konfigurasi platform tunggal.
- Pemilihan driver SPL secara compile-time agar muat dalam Flash 8 KB.
- Pemisahan application dan platform/hardware.
- Contoh UART1, delay TIM4, dan LED bawaan yang siap dijalankan.
- Driver periferal terpisah agar façade platform tetap ringkas.
- Scaffolding seluruh periferal STM8S103F3P6.

> Status: layak digunakan sebagai template internal/eksperimental. Sebelum dipublikasikan sebagai template stabil, tentukan lisensi, tambahkan CI, dan lengkapi pengujian driver yang akan dinyatakan stabil.

## Hardware dan toolchain

| Komponen | Nilai |
|---|---|
| MCU | STM8S103F3P6 |
| Clock default | HSI 16 MHz |
| Flash | 8 KB |
| RAM | 1 KB |
| Framework | STM8 SPL 2.3.1 |
| PlatformIO platform | `ststm8@2.1.0` |
| Compiler | SDCC |
| Programmer | ST-LINK V2 / SWIM |
| Serial | UART1, default 9600 baud |

## Memulai proyek baru

Cara yang disarankan setelah repository ditandai sebagai **Template repository** di GitHub adalah memilih **Use this template**. Alternatifnya, clone repository lalu ganti remote:

```powershell
git clone <URL_TEMPLATE> nama-proyek-baru
cd nama-proyek-baru
git remote remove origin
git remote add origin <URL_REPOSITORY_BARU>
```

Setelah itu:

1. Buka folder proyek di VS Code dengan ekstensi PlatformIO.
2. Pilih periferal di `include/platform/platform_features.h`.
3. Ganti use case contoh di `src/application/app.c`.
4. Build firmware dengan `pio run`.
5. Upload melalui ST-LINK dengan `pio run --target upload`.
6. Buka UART melalui adaptor USB-to-TTL dengan `pio device monitor`.

## Wiring

### ST-LINK V2

Hubungkan minimal `SWIM` dan `GND`; sambungkan reset dan referensi tegangan sesuai board/programmer yang digunakan. Hindari memberi daya dari dua sumber yang tidak kompatibel.

### UART1

| STM8S103 | USB-to-TTL |
|---|---|
| PD5 / TX | RX |
| PD6 / RX | TX |
| GND | GND |

Pastikan level tegangan USB-to-TTL sesuai dengan tegangan kerja MCU.

## Konfigurasi periferal

Edit `include/platform/platform_features.h`:

```c
#define STM8_FEATURE_CLK    1
#define STM8_FEATURE_GPIO   1
#define STM8_FEATURE_TIM4   1
#define STM8_FEATURE_UART1  1

#define STM8_FEATURE_ADC1   0
#define STM8_FEATURE_I2C    0
#define STM8_FEATURE_SPI    0
```

- `1`: driver SPL dan adapter platform ikut dibangun.
- `0`: periferal tetap terdefinisi dalam platform, tetapi implementasinya tidak memakan Flash.

Setelah mengubah feature:

```powershell
pio run --target clean
pio run
```

Memasukkan seluruh driver sekaligus menghasilkan firmware sekitar 11,5 KB, melebihi Flash 8 KB. Karena itu pemilihan compile-time merupakan bagian penting dari desain template ini.

## Objek platform

Seluruh platform disatukan oleh satu objek di `src/main.c`:

```c
static const stm8_platform_t platform = STM8_PLATFORM_DEFAULT;
```

Konfigurasi manual dapat dibuat seperti berikut:

```c
static const stm8_platform_t platform = {
    STM8_PLATFORM_ENABLED_MASK,
    115200UL, /* UART1 baud rate */
    124U      /* TIM4: tick 1 ms pada HSI 16 MHz */
};
```

Feature compile-time menentukan kode yang masuk ke firmware, sedangkan field `enabled` menentukan periferal yang diaktifkan saat startup.

## Aplikasi contoh

Implementasi bawaan di `src/application/app.c`:

- Mengirim `STM8S template ready` sekali saat startup.
- Mengirim `Application heartbeat` setiap sekitar satu detik.
- Membalik kondisi LED bawaan PB5 setiap satu detik. Karena satu siklus terdiri
  dari ON dan OFF, periode kedip lengkapnya sekitar dua detik.

File tersebut sengaja sederhana dan merupakan tempat pertama yang diganti saat membuat proyek baru.

## Pembagian kode platform

```text
src/platform/
├── stm8_platform.c       # façade dan urutan inisialisasi
├── board/
│   └── stm8sblue_board.c # pin/resource khusus board, termasuk LED PB5
└── drivers/
    ├── clock_driver.c
    ├── tim4_delay.c
    ├── uart1_driver.c
    └── ...               # satu modul untuk setiap periferal
```

Application hanya menggunakan API `stm8_platform_*`. Detail SPL berada dalam
driver dan detail pin board berada dalam modul `board`.

## Dokumentasi

- [Struktur dan arsitektur](docs/ARCHITECTURE.md)
- [Referensi fungsi/API](docs/API.md)
- [Daftar periferal](docs/PERIPHERALS.md)
- [Panduan membuat fitur baru](docs/ADDING_A_FEATURE.md)
- [Checklist publikasi template](docs/TEMPLATE_CHECKLIST.md)

## Perintah utama

```powershell
# Build
pio run

# Clean build
pio run --target clean
pio run

# Upload
pio run --target upload

# Serial monitor
pio device monitor
```

## Aturan resource

- Hindari `malloc()` dan heap.
- Gunakan state modul `static` dan buffer kecil.
- Hindari `printf()` jika fungsi konversi sederhana mencukupi.
- Aktifkan hanya driver SPL yang digunakan.
- Jangan melakukan proses berat di ISR.
- Periksa penggunaan Flash/RAM pada setiap build.
- IWDG dan WWDG tidak dapat dimatikan kembali melalui software setelah aktif.

## Lisensi

Belum ditentukan. Pilih dan tambahkan file `LICENSE` sebelum repository dipublikasikan atau digunakan pihak lain.
