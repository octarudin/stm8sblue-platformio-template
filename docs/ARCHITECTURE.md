# Struktur dan Arsitektur

## Tujuan

Template memisahkan aturan aplikasi dari detail STM8/SPL tanpa heap,
dependency-injection container, atau abstraksi berat.

```text
main.c
├── application
│   └── cooperative scheduler
└── stm8_platform façade
    ├── board STM8SBlue
    └── peripheral drivers
        └── STM8 SPL
            └── register hardware
```

## Struktur saat ini

```text
.
├── platformio.ini
├── README.md
├── docs/
├── include/
│   ├── application/
│   │   └── app.h
│   ├── kernel/
│   │   ├── cooperative_scheduler.h
│   │   └── scheduler_config.h
│   └── platform/
│       ├── stm8_platform.h
│       ├── platform_features.h
│       ├── board/
│       │   └── stm8sblue_board.h
│       └── drivers/
│           ├── clock_driver.h
│           ├── tim4_time.h
│           ├── uart1_driver.h
│           └── ...
├── src/
│   ├── main.c
│   ├── stm8s_conf.h
│   ├── application/
│   │   └── app.c
│   ├── kernel/
│   │   └── cooperative_scheduler.c
│   └── platform/
│       ├── stm8_platform.c
│       ├── board/
│       │   └── stm8sblue_board.c
│       └── drivers/
│           ├── clock_driver.c
│           ├── tim4_time.c
│           ├── uart1_driver.c
│           └── ...
├── lib/
└── test/
```

## Tanggung jawab setiap bagian

### `src/main.c`

Composition root firmware. File ini membuat objek platform, menginisialisasi
platform dan application, lalu menjalankan main loop. Jangan menaruh konfigurasi
register atau business logic di sini.

### `src/application/`

Berisi use case dan alur aplikasi. Contoh saat ini mendaftarkan task heartbeat
UART dan LED ke scheduler. Application menggunakan API façade
`stm8_platform_*`, memakai scheduler sebagai layanan kernel, dan tidak memanggil
SPL secara langsung.

### `src/kernel/`

Berisi cooperative scheduler yang bebas dari SPL dan detail STM8. Scheduler
menyimpan tabel task statis, memeriksa deadline 16-bit, dan memanggil callback
dari main loop. Scheduler menerima nilai waktu dari platform sehingga logikanya
tidak memiliki dependensi hardware.

### `src/platform/stm8_platform.c`

Façade platform dan pengatur urutan startup. Tanggung jawabnya hanya:

- memeriksa feature compile-time dan bitmask runtime;
- memanggil fungsi init driver yang sesuai;
- meneruskan API application ke driver atau board.

Implementasi SPL tidak diletakkan dalam file ini.

### `src/platform/drivers/`

Satu modul memiliki satu periferal atau satu layanan hardware. Contoh:

- `clock_driver.c`: konfigurasi HSI;
- `uart1_driver.c`: konfigurasi dan TX UART1;
- `tim4_time.c`: clock sistem 1 ms, ISR TIM4, dan delay blocking;
- `watchdog_driver.c`: IWDG dan WWDG.

Hanya layer ini dan layer board yang boleh memanggil fungsi SPL.

### `src/platform/board/`

Berisi detail fisik board, bukan periferal MCU generik. Pada STM8SBlue, modul
ini memiliki LED bawaan di PB5. Jika template dipindahkan ke board lain, pemetaan
pin diubah di sini tanpa mengubah application.

### `src/stm8s_conf.h`

Menentukan source SPL yang ikut dibangun berdasarkan `STM8_FEATURE_*`. File ini
wajib berada langsung di root `src/` karena builder PlatformIO STM8 SPL 2.1.0
mencarinya pada lokasi tersebut.

### `include/`

Berisi kontrak fungsi publik/internal proyek. Jangan menaruh state global atau
implementasi fungsi besar di header.

### `docs/`, `lib/`, dan `test/`

- `docs/`: arsitektur, API, periferal, dan panduan kontribusi.
- `lib/`: library yang benar-benar reusable lintas proyek.
- `test/`: pengujian host, target, mock, atau hardware-in-the-loop.

## Aturan penempatan kode

| Kebutuhan | Lokasi |
|---|---|
| Membuat objek platform dan main loop | `src/main.c` |
| Use case/business logic | `src/application/*.c` |
| Interface application | `include/application/*.h` |
| Scheduler dan konfigurasi task | `src/kernel/` dan `include/kernel/` |
| Façade dan urutan init | `src/platform/stm8_platform.c` |
| Interface façade | `include/platform/stm8_platform.h` |
| Implementasi satu periferal SPL | `src/platform/drivers/*.c` |
| Interface driver | `include/platform/drivers/*.h` |
| Pin/resource khusus board | `src/platform/board/*.c` |
| Memilih source SPL | `src/stm8s_conf.h` |
| Mengaktifkan feature | `include/platform/platform_features.h` |
| Aturan/perhitungan bebas hardware | `src/domain/*.c` jika diperlukan |

## Arah dependensi

```text
application → scheduler + stm8_platform façade
scheduler   → tidak bergantung pada hardware
platform    → board/drivers → SPL
domain      → tidak bergantung pada hardware
main        → application + stm8_platform façade
```

Application saat ini menerima `stm8_platform_t` secara langsung sebagai
kompromi lightweight. Tambahkan application port terpisah hanya jika pengujian
mock atau dukungan platform kedua benar-benar diperlukan.

## Kepemilikan resource

Satu pin atau timer hanya boleh dimiliki satu modul:

- PB5: `stm8sblue_board.c`, LED bawaan;
- TIM4: `tim4_time.c`, clock scheduler dan layanan delay;
- UART1: `uart1_driver.c`, serial TX.

Jangan mengubah konfigurasi resource tersebut dari application atau driver lain
tanpa memindahkan kepemilikannya dan memperbarui dokumentasi.

## Penempatan variabel

- Variabel sementara: lokal di fungsi.
- State privat modul: `static` di file `.c` pemiliknya.
- Konfigurasi tetap: `static const`, biasanya di `main.c`.
- Data yang dibagi dengan ISR: `volatile`, dengan critical section bila perlu.

ISR sebaiknya membersihkan flag, menyimpan data minimum, dan memberi tanda ke
main loop. Jangan menjalankan business logic atau operasi blocking di ISR.

Callback scheduler juga berjalan di main loop, bukan ISR. Karena scheduler
bersifat cooperative, setiap callback harus cepat selesai dan tidak boleh
memakai delay blocking.
