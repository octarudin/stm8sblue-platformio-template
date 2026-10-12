#ifndef KERNEL_SCHEDULER_CONFIG_H
#define KERNEL_SCHEDULER_CONFIG_H

/* Setiap slot memakai function pointer, period, deadline, dan status aktif. */
#define SCHEDULER_MAX_TASKS 6U

/* Batas ini menjaga perbandingan waktu 16-bit tetap aman saat wrap-around. */
#define SCHEDULER_MAX_INTERVAL_MS 32767U

#endif
