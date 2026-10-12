#include "platform/drivers/uart1_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_UART1
#include "stm8s.h"

void uart1_driver_init(uint32_t baud_rate)
{
    UART1_DeInit();
    UART1_Init(
        baud_rate,
        UART1_WORDLENGTH_8D,
        UART1_STOPBITS_1,
        UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE,
        UART1_MODE_TX_ENABLE
    );
    UART1_Cmd(ENABLE);
}

void uart1_driver_write(const char *text)
{
    while (*text != '\0') {
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET) {
        }
        UART1_SendData8((uint8_t)*text++);
    }
}
#endif
