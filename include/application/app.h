#ifndef APPLICATION_APP_H
#define APPLICATION_APP_H

#include "platform/stm8_platform.h"

/**
 * Menyiapkan state application. Dipanggil satu kali setelah platform siap.
 */
void app_init(const stm8_platform_t *platform);

/**
 * Menjalankan satu iterasi use case contoh.
 *
 * Implementasi bawaan menjalankan task LED dan heartbeat melalui cooperative
 * scheduler. Fungsi tidak melakukan delay dan harus dipanggil sesering mungkin.
 */
void app_run_once(const stm8_platform_t *platform);

#endif
