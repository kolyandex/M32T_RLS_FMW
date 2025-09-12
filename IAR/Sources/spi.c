#include "spi.h"
#include "derivative.h" /* include peripheral declarations */
#include "lin_hw_cfg.h"

static void delay(int result)
{
    for (; result; --result)
        ;
}

void set_spi_baud(unsigned int a1, unsigned int a2)
{
    unsigned int v2;
    signed int v3;
    unsigned char v4;
    v2 = a1 / a2;
    v3 = 0;
LABEL_2:
    v4 = 0;
    while (v4 + 1 < v2 >> (v3 + 1))
    {
        if (++v4 >= 8)
        {
            if (++v3 <= 8)
                goto LABEL_2;
            break;
        }
    }
    if (v4 >= 8)
        v4 = 7;
    if (v3 > 8)
    {
        v3 &= 0xFFFFFF00;
        v3 |= 0x08;
    }
    SPI0_BR = v3 & 0xF | 16 * v4 & 0x70;
}
void spi_sleep(void)
{
    SPI0_C1 &= ~SPI_C1_SPE_MASK;
}
void spi_wakeup(void)
{
    SPI0_C1 |= SPI_C1_SPE_MASK;
}
void spi_init(void)
{
    SIM_SCGC |= SIM_SCGC_SPI0_MASK;
    SPI0_C1 |= SPI_C1_MSTR_MASK;
    SPI0_C1 |= SPI_C1_CPOL_MASK;
    SPI0_C1 |= SPI_C1_CPHA_MASK;
    set_spi_baud(MCU_BUS_FREQ, 1000000);
    SPI0_C1 |= SPI_C1_SPE_MASK;
}
int spi_tx_rx(int d)
{
    signed int v1;
    while (!(SPI0_S & SPI_S_SPTEF_MASK))
        ;
    SPI0_D = d;
    v1 = 0x33;
    do
    {
        if (SPI0_S & SPI_S_SPRF_MASK)
            break;
        delay(5);
        --v1;
    } while (v1);
    return SPI0_D;
}