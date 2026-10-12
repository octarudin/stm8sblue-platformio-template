#ifndef PLATFORM_BOARD_STM8SBLUE_BOARD_H
#define PLATFORM_BOARD_STM8SBLUE_BOARD_H

/** Menyiapkan resource khusus board, termasuk LED bawaan PB5. */
void stm8sblue_board_init(void);

/** Membalik kondisi LED bawaan board. */
void stm8sblue_board_led_toggle(void);

#endif
