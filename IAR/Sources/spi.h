#pragma once


void spi_init(void);
int spi_tx_rx(int);
void spi_sleep(void);
void spi_wakeup(void);