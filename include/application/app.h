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
 * Implementasi bawaan mengirim pesan UART setiap satu detik dan dimaksudkan
 * sebagai titik awal yang diganti oleh aplikasi pengguna template.
 */
void app_run_once(const stm8_platform_t *platform);

#endif
