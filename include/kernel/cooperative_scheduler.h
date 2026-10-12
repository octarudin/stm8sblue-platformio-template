#ifndef KERNEL_COOPERATIVE_SCHEDULER_H
#define KERNEL_COOPERATIVE_SCHEDULER_H

#include <stdint.h>
#include "kernel/scheduler_config.h"

typedef void (*scheduler_task_fn_t)(void);
typedef uint8_t scheduler_task_id_t;

#define SCHEDULER_INVALID_TASK_ID ((scheduler_task_id_t)0xFFU)

/** Mengosongkan tabel task dan menetapkan referensi waktu pendaftaran. */
void scheduler_init(uint16_t now_ms);

/**
 * Mendaftarkan task periodik tanpa alokasi heap.
 *
 * period_ms dan initial_delay_ms harus berada pada rentang 1..32767 ms.
 * Task pertama kali berjalan setelah initial_delay_ms, kemudian periodik.
 *
 * @return ID task, atau SCHEDULER_INVALID_TASK_ID jika argumen tidak valid
 *         atau tabel sudah penuh.
 */
scheduler_task_id_t scheduler_add(
    scheduler_task_fn_t task,
    uint16_t period_ms,
    uint16_t initial_delay_ms
);

/**
 * Mengaktifkan atau menonaktifkan task yang sudah terdaftar.
 * Task yang diaktifkan kembali dijadwalkan setelah satu periodenya.
 *
 * @return 1 jika ID valid, 0 jika tidak valid.
 */
uint8_t scheduler_set_enabled(
    scheduler_task_id_t task_id,
    uint8_t enabled,
    uint16_t now_ms
);

/** Menjalankan setiap task yang sudah jatuh tempo paling banyak satu kali. */
void scheduler_run(uint16_t now_ms);

/** Mengembalikan jumlah slot task yang sudah terdaftar. */
uint8_t scheduler_task_count(void);

#endif
