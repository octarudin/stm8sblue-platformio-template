# Cooperative Scheduler

## Tujuan

Scheduler menjalankan beberapa task periodik dalam satu main loop tanpa RTOS,
heap, context switch, atau stack per task. Desain ini ditujukan untuk
STM8S103F3P6 yang hanya memiliki RAM 1 KB.

Scheduler bersifat cooperative: sebuah task berjalan sampai fungsi task
tersebut selesai dan kembali ke scheduler. Tidak ada preemption antar-task.

## Alur eksekusi

```text
TIM4 interrupt setiap 1 ms
        |
        v
system_time_ms bertambah
        |
        v
app_run_once()
        |
        v
scheduler_run(now_ms)
        |
        +-- jalankan task LED jika jatuh tempo
        +-- jalankan task heartbeat jika jatuh tempo
        +-- kembali ke main loop
```

ISR TIM4 hanya membersihkan flag dan menambah counter. Callback task selalu
dijalankan dari main loop, bukan dari interrupt.

## Konfigurasi

Edit `include/kernel/scheduler_config.h`:

```c
#define SCHEDULER_MAX_TASKS       6U
#define SCHEDULER_MAX_INTERVAL_MS 32767U
```

Tabel task dialokasikan statis. Menaikkan `SCHEDULER_MAX_TASKS` menambah
pemakaian RAM meskipun semua slot belum digunakan.

Scheduler memerlukan:

```c
#define STM8_FEATURE_CLK  1
#define STM8_FEATURE_TIM4 1
```

Konfigurasi default TIM4 memakai HSI 16 MHz, prescaler 128, dan auto-reload 124
untuk menghasilkan tick nominal 1 ms.

## Mendaftarkan task

Callback tidak menerima parameter agar setiap slot tetap kecil. State atau
dependency task disimpan sebagai state privat modul.

```c
static const stm8_platform_t *active_platform;

static void led_task(void)
{
    stm8_platform_led_toggle(active_platform);
}

void app_init(const stm8_platform_t *platform)
{
    scheduler_task_id_t task_id;

    active_platform = platform;
    scheduler_init(stm8_platform_millis(platform));

    task_id = scheduler_add(
        led_task,
        1000U, /* period */
        1000U  /* initial delay */
    );

    if (task_id == SCHEDULER_INVALID_TASK_ID) {
        /* Tangani tabel penuh atau parameter tidak valid. */
    }
}
```

`period_ms` dan `initial_delay_ms` harus berada pada rentang 1 sampai 32767 ms.
Nilai lebih panjang dapat dibentuk dengan counter di dalam task.

## Menjalankan scheduler

Panggil scheduler sesering mungkin:

```c
void app_run_once(const stm8_platform_t *platform)
{
    scheduler_run(stm8_platform_millis(platform));
}
```

Jangan menambahkan delay pada main loop. Ketepatan dispatch bergantung pada
seberapa cepat kontrol kembali ke `scheduler_run()`.

## Mengaktifkan dan menonaktifkan task

Simpan ID hasil pendaftaran:

```c
static scheduler_task_id_t sensor_task_id;

sensor_task_id = scheduler_add(sensor_task, 100U, 100U);
```

Nonaktifkan atau aktifkan kembali:

```c
scheduler_set_enabled(sensor_task_id, 0U, now_ms);
scheduler_set_enabled(sensor_task_id, 1U, now_ms);
```

Ketika diaktifkan kembali, task dijadwalkan setelah satu periode penuh.

## Aturan callback task

Callback task harus:

- selesai secepat mungkin;
- tidak memanggil `stm8_platform_delay_ms()`;
- tidak memiliki loop tunggu tanpa timeout;
- tidak melakukan business logic di ISR;
- menyimpan state persisten sebagai `static` atau dalam modul pemiliknya;
- tidak mengubah resource hardware milik task/modul lain tanpa koordinasi.

UART1 saat ini menggunakan polling dan tetap blocking. Selama pengiriman, task
lain belum dapat berjalan, tetapi interrupt TIM4 tetap memperbarui waktu.

## Task terlambat

Jika task terlambat beberapa periode, scheduler:

1. menjalankan task tersebut paling banyak satu kali pada satu dispatch;
2. melewati eksekusi yang terlewat;
3. mempertahankan fase deadline periodik berikutnya.

Perilaku ini mencegah burst callback setelah operasi yang lama.

## Wrap-around waktu

Clock menggunakan `uint16_t` dan wrap setiap 65,536 detik. Perbandingan deadline
tetap aman selama:

- period dan initial delay tidak melebihi 32767 ms;
- scheduler dipanggil kembali sebelum berhenti lebih dari 32767 ms.

Jangan membandingkan waktu dengan `now >= deadline` secara langsung. Gunakan
scheduler atau selisih unsigned yang memang dirancang untuk wrap-around.

## Batasan

- Tidak ada priority; urutan eksekusi mengikuti urutan pendaftaran.
- Tidak ada preemption atau stack terpisah.
- Tidak ada penghapusan slot task; task dapat dinonaktifkan.
- Callback menggunakan signature `void task(void)`.
- Task yang lama menambah latency semua task setelahnya.
- Ketepatan tick mengikuti akurasi clock HSI MCU.

Scheduler ini cocok untuk LED, polling tombol, pembacaan sensor periodik,
telemetry, dan state machine ringan. Gunakan RTOS hanya jika kebutuhan aplikasi
benar-benar memerlukan preemption, blocking primitive, atau priority kompleks.
