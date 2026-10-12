#ifndef __STM8S_CONF_H
#define __STM8S_CONF_H

#include "../include/platform/platform_features.h"
#include "stm8s.h"

#if STM8_FEATURE_ADC1
#include "stm8s_adc1.h"
#endif
#if STM8_FEATURE_AWU
#include "stm8s_awu.h"
#endif
#if STM8_FEATURE_BEEP
#include "stm8s_beep.h"
#endif
#if STM8_FEATURE_CLK
#include "stm8s_clk.h"
#endif
#if STM8_FEATURE_EXTI
#include "stm8s_exti.h"
#endif
#if STM8_FEATURE_FLASH
#include "stm8s_flash.h"
#endif
#if STM8_FEATURE_GPIO
#include "stm8s_gpio.h"
#endif
#if STM8_FEATURE_I2C
#include "stm8s_i2c.h"
#endif
#if STM8_FEATURE_ITC
#include "stm8s_itc.h"
#endif
#if STM8_FEATURE_IWDG
#include "stm8s_iwdg.h"
#endif
#if STM8_FEATURE_RST
#include "stm8s_rst.h"
#endif
#if STM8_FEATURE_SPI
#include "stm8s_spi.h"
#endif
#if STM8_FEATURE_TIM1
#include "stm8s_tim1.h"
#endif
#if STM8_FEATURE_TIM2
#include "stm8s_tim2.h"
#endif
#if STM8_FEATURE_TIM4
#include "stm8s_tim4.h"
#endif
#if STM8_FEATURE_UART1
#include "stm8s_uart1.h"
#endif
#if STM8_FEATURE_WWDG
#include "stm8s_wwdg.h"
#endif

/* Aktifkan bila ingin pemeriksaan parameter SPL. */
/* #define USE_FULL_ASSERT */

#ifdef USE_FULL_ASSERT

#define assert_param(expr) \
    ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t *file, uint32_t line);

#else

#define assert_param(expr) ((void)0)

#endif

#endif
