# Menambahkan Fitur Aplikasi

Panduan ini menggunakan contoh pembacaan ADC dan pengiriman hasil melalui UART.

## 1. Aktifkan driver yang diperlukan

Edit `include/platform/platform_features.h`:

```c
#define STM8_FEATURE_ADC1   1
#define STM8_FEATURE_UART1  1
```

Tidak perlu mengedit `stm8s_conf.h`; header SPL akan dipilih otomatis.

## 2. Buat driver platform

Buat interface:

```text
include/platform/stm8/adc1_driver.h
```

```c
#ifndef STM8_ADC1_DRIVER_H
#define STM8_ADC1_DRIVER_H

#include <stdint.h>

void adc1_driver_init(void);
uint16_t adc1_driver_read(uint8_t channel);

#endif
```

Buat implementasi:

```text
src/platform/stm8/adc1_driver.c
```

Implementasi ini boleh memasukkan `stm8s.h` dan menggunakan fungsi `ADC1_*`.
Tambahkan pemanggilan init-nya ke `stm8_platform_init()`; jangan menaruh
implementasi register atau SPL langsung di `stm8_platform.c`.

Untuk resource yang khusus board, seperti LED bawaan atau pemetaan konektor,
gunakan `include/platform/board/` dan `src/platform/board/`. Driver menjelaskan
cara mengendalikan periferal MCU, sedangkan modul board menentukan pin yang
digunakan board.

## 3. Tambahkan domain jika ada perhitungan

Untuk konversi ADC ke satuan fisik:

```text
include/domain/measurement.h
src/domain/measurement.c
```

Domain menerima angka dan mengembalikan hasil. Domain tidak mengetahui ADC1 atau STM8.

## 4. Tambahkan use case

Buat atau ubah module di `src/application/`. Use case melakukan urutan:

1. Meminta pembacaan melalui interface/driver.
2. Memproses nilai melalui domain.
3. Mengirim hasil melalui serial.

Jangan menulis konfigurasi register dalam application.

Jika use case berjalan periodik, daftarkan callback singkat ke cooperative
scheduler dari `app_init()`:

```c
scheduler_add(sensor_task, 100U, 100U);
```

Callback task tidak boleh menggunakan `stm8_platform_delay_ms()`. Simpan state
antar-eksekusi sebagai state privat modul dan segera kembalikan kontrol ke
scheduler.

## 5. Hubungkan di composition root

Panggil fungsi init hardware dari `stm8_platform_init()`.

- Inisialisasi hardware: platform.
- Inisialisasi state use case: application.
- Pemilihan objek/modul: `main.c`.

## 6. Build dan periksa ukuran

```powershell
pio run --target clean
pio run
```

Pastikan firmware tetap di bawah 8192 byte dan perhatikan penggunaan RAM.

## 7. Uji hardware

Minimal verifikasi:

- Nilai normal.
- Nilai batas bawah/atas.
- Periferal tidak terhubung.
- Timeout atau bus macet jika relevan.
- Interaksi dengan UART/timer/periferal lain.
- Waktu eksekusi task dan respons task lain ketika use case berjalan.

## Definition of done

Sebuah feature dianggap selesai jika:

- Interface dan implementasi dipisahkan.
- Application tidak mengakses SPL secara langsung.
- Pin dan resource timer didokumentasikan.
- Callback scheduler tidak blocking dan memenuhi periodenya.
- Build berhasil dengan warning yang dipahami.
- Ukuran firmware masih muat.
- Perilaku diuji pada hardware.
- Dokumentasi API diperbarui.
