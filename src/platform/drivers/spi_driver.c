#include "platform/drivers/spi_driver.h"
#include "platform/platform_features.h"

#if STM8_FEATURE_SPI
#include "stm8s.h"

void spi_driver_init(void)
{
    SPI_DeInit();
    SPI_Init(
        SPI_FIRSTBIT_MSB,
        SPI_BAUDRATEPRESCALER_16,
        SPI_MODE_MASTER,
        SPI_CLOCKPOLARITY_LOW,
        SPI_CLOCKPHASE_1EDGE,
        SPI_DATADIRECTION_2LINES_FULLDUPLEX,
        SPI_NSS_SOFT,
        7U
    );
    SPI_Cmd(ENABLE);
}
#endif
