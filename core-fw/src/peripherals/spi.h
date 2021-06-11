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


#ifndef PERIPHERALS_SPI_H_
#define PERIPHERALS_SPI_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "utilities/lock.h"
#include "gpio.h"
#include "stm32f3xx_hal_spi.h"

#include <stdint.h>
#include <stddef.h>
#include <stm32f3xx.h>

//=================================
// declarations

struct spi_t {
	mutex mtx;
	SPI_HandleTypeDef* perif;
};
struct spi_dev_t {
	struct spi_t* spi;
	IO_pin_t nss;
};

#define SPI_DEV_TPL(cs_pin, spi_def) {spi_def, cs_pin}

extern struct spi_t spi1;

extern int spi_lock(struct spi_dev_t* spid);
extern void spi_unlock(struct spi_dev_t* spid);

extern void spi_start(struct spi_dev_t* spid);
extern void spi_stop(struct spi_dev_t* spid);
extern int spi_ready(struct spi_dev_t* spid);

//8-bit transceive functions (send & receive)
extern void spi_xfer8(struct spi_dev_t* spid, uint8_t* data_tx,
		uint8_t* data_rx, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PERIPHERALS_SPI_H_ */
