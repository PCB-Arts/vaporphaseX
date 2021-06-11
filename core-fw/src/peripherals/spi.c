/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


//=================================
// included dependencies
#include "peripherals/spi.h"
#include "stm32f3xx_hal_spi.h"

#include "conf.h"

//=================================
// definitions

struct spi_t spi1;

int spi_lock(struct spi_dev_t* spid) {
	return lock(&spid->spi->mtx);
}

void spi_unlock(struct spi_dev_t* spid) {
	unlock(&spid->spi->mtx);
}

void spi_start(struct spi_dev_t* spid) {
	//activate ship select = low
	iopin_write(spid->nss, 0);
}

void spi_stop(struct spi_dev_t* spid) {
	//deactivate ship select = high
	iopin_write(spid->nss, 1);
}

#if !MOCK_CORE

int spi_ready(struct spi_dev_t* spid) {
	return HAL_SPI_GetState(spid->spi->perif) == HAL_SPI_STATE_READY;
}

void spi_xfer8(struct spi_dev_t* spid, uint8_t* data_tx,
		uint8_t* data_rx, size_t len) {
	HAL_SPI_TransmitReceive_DMA(spid->spi->perif, data_tx, data_rx, len);
}

#endif
