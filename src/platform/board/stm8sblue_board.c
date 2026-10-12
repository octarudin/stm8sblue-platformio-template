#include "platform/board/stm8sblue_board.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_GPIO
#include "stm8s.h"

#define BOARD_LED_PORT GPIOB
#define BOARD_LED_PIN  GPIO_PIN_5

void stm8sblue_board_init(void)
{
    /* LED bawaan active-low; HIGH membuat LED mati saat startup. */
    GPIO_Init(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void stm8sblue_board_led_toggle(void)
{
    GPIO_WriteReverse(BOARD_LED_PORT, BOARD_LED_PIN);
}
#endif
