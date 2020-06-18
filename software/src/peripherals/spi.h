/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2019 PCB Arts GmbH - Kevin Schick
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  _____    ____    ____
 *       \  /       |    \        /\            |
 *   ____/  |       |____/       /  \     __  __|__   ___
 *  |       |       |    \      /____\   /      |    |___
 *  |       \____   |____/     /      \  |      |     ___|
 *
 *         _    __   __   __     __         _    __   __
 *  \  /  /_\  |__| |  | |__|   |__| |__|  /_\  |__  |__
 *   \/  /   \ |    |__| | \    |    |  | /   \  __| |__
 *
 *
 *  PCB Arts GmbH
 *  Kurgartenstrasse 59
 *  90762 Fuerth
 *  GERMANY
 *
 *  www.pcb-arts.com
 *  mail(at)pcb-arts.com
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


#ifndef PERIPHERALS_SPI_H_
#define PERIPHERALS_SPI_H_

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


//=================================
// included dependencies
#include "light_hal/io.h"
#include "utilities/lock.h"

#include <stdint.h>
#include <stddef.h>
#include <stm32f3xx.h>

//=================================
// declarations

struct dev_spi_dma_t {
	DMA_Channel_TypeDef* ch;
	uint32_t iqrn;
};

struct spi_t {
	mutex mtx;
	SPI_TypeDef* perif;
	struct dev_spi_dma_t dma_tx;
	struct dev_spi_dma_t dma_rx;
};

struct spi_dev_t {
	struct spi_t* spi;
	IO_pin_t nss;
};

#define SPI_DEV_TPL(cs_pin, spi_def) {spi_def, cs_pin}


extern struct spi_t spi2, spi3;


extern void spi_init(struct spi_t* spid);
extern void spi_mode(struct spi_dev_t* spid, uint8_t cpol, uint8_t cpah);

extern int spi_lock(struct spi_dev_t* spid);
extern void spi_unlock(struct spi_dev_t* spid);

extern void spi_start(struct spi_dev_t* spid);
extern void spi_stop(struct spi_dev_t* spid);
extern int spi_ready(struct spi_dev_t* spid);
extern void spi_wait(struct spi_dev_t* spid);

extern void spi_set8(struct spi_dev_t* spid);
extern void spi_set16(struct spi_dev_t* spid);

//8-bit send/receive functions
extern void spi_send8(struct spi_dev_t* spid, __IO uint8_t* data, size_t len);
extern void spi_sendCircular8(struct spi_dev_t* spid, __IO uint8_t* data, size_t len);
extern void spi_recv8(struct spi_dev_t* spid, __IO uint8_t* data, size_t len);

//16-bit functions
extern void spi_send16(struct spi_dev_t* spid, __IO uint16_t *data, size_t len);
extern void spi_sendCircular16(struct spi_dev_t* spid, __IO uint16_t* data, size_t len);
extern void spi_recv16(struct spi_dev_t* spid, __IO uint16_t* data, size_t len);

//8-bit transceive functions (send & receive)
extern void spi_xfer8(struct spi_dev_t* spid, __IO uint8_t* data_tx,
		__IO uint8_t* data_rx, size_t len);

extern void spi_xfer16(struct spi_dev_t* spid, __IO uint16_t* data_tx,
		__IO uint16_t* data_rx, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PERIPHERALS_SPI_H_ */
