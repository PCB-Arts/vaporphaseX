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


//=================================
// included dependencies
#include "peripherals/spi.h"

//=================================
// definitions

static void prep_dma_8bit(struct dev_spi_dma_t* dma);
static void prep_dma_16bit(struct dev_spi_dma_t* dma);

static void send(struct spi_t* spi, __IO uint8_t* data, size_t len);
static void send_circular(struct spi_t* spi, __IO uint8_t* data, size_t len);
static void recv(struct spi_t* spi, __IO uint8_t* data, size_t len);


struct spi_t spi2, spi3;


void spi_init(struct spi_t* spi) {
	//enable peripheral clock
	//reset peripheral
	//enable interrupt line

	if (!spi)
		return;

	if (spi->perif == SPI2) {
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
		SET_BIT(RCC->APB2RSTR, RCC_APB1RSTR_SPI2RST);
		NVIC_EnableIRQ(SPI2_IRQn);
	} else if (spi->perif == SPI3) {
		SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3EN);
		SET_BIT(RCC->APB2RSTR, RCC_APB1RSTR_SPI3RST);
		NVIC_EnableIRQ(SPI3_IRQn);
	}

	MODIFY_REG(spi->perif->CR2,
		////////// clear bits //////////
		//clear data size
		SPI_CR2_DS |

		//TXE interrupt masked
		SPI_CR2_TXEIE |

		//RXNE interrupt masked
		SPI_CR2_RXNEIE |

		//Error interrupt is masked
		SPI_CR2_ERRIE |

		//SPI Motorola mode
		SPI_CR2_FRF |

		//No NSS pulse
		SPI_CR2_NSSP |

		//SS output is disabled
		SPI_CR2_SSOE,

		////////// set bits //////////
		//Tx DMA enabled
		SPI_CR2_TXDMAEN |

		//Rx DMA enabled
		SPI_CR2_RXDMAEN |

		//data size 0111: 8-bit (bit pos. 8)
		(0b0111 << 8) |

		//RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
		SPI_CR2_FRXTH);

	MODIFY_REG(spi->perif->CR1,
		////////// clear bits //////////
		//2-line unidirectional data mode selected
		SPI_CR1_BIDIMODE |

		//CRC calculation disabled
		SPI_CR1_CRCEN |

		//Full-duplex (Transmit and receive)
		SPI_CR1_RXONLY |

		//data is transmitted / received with the MSB first
		SPI_CR1_LSBFIRST |

		//clear Baud rate control
		SPI_CR1_BR |

		//CK to 0 when idle
		SPI_CR1_CPOL,

		////////// set bits //////////
		//Software slave management enabled
		SPI_CR1_SSM |

		//Internal slave select enabled
		SPI_CR1_SSI |

		//Baud rate control 100: fPCLK/32 (bit pos. 3)
		(0b100 << 3) |

		//Master configuration
		SPI_CR1_MSTR |

		//the second clock transition (edge) is the first data capture edge
		SPI_CR1_CPHA |

		//enabled SPI module
		SPI_CR1_SPE);
}

void spi_mode(struct spi_dev_t* spid, uint8_t cpol, uint8_t cpah) {
	//disable peripheral
	CLEAR_BIT(spid->spi->perif->CR1, SPI_CR1_SPE);

	MODIFY_REG(spid->spi->perif->CR1,
		////////// clear bits //////////
		SPI_CR1_CPOL |
		SPI_CR1_CPHA,

		////////// set bits //////////
		(cpol ? 1 << SPI_CR1_CPOL_Pos : 0) |
		(cpah ? 1 << SPI_CR1_CPHA_Pos : 0));

	//enable peripheral
	SET_BIT(spid->spi->perif->CR1, SPI_CR1_SPE);
}

int spi_lock(struct spi_dev_t* spid) {
	return lock(&spid->spi->mtx);
}

void spi_unlock(struct spi_dev_t* spid) {
	unlock(&spid->spi->mtx);
}

void spi_start(struct spi_dev_t* spid) {
	//activate ship select = low
	IOPIN_WRITE(spid->nss, 0);
}

void spi_stop(struct spi_dev_t* spid) {
	//deactivate ship select = high
	IOPIN_WRITE(spid->nss, 1);
}

int spi_ready(struct spi_dev_t* spid) {
	return !(spid->spi->perif->SR & SPI_SR_BSY);
}

void spi_wait(struct spi_dev_t* spid) {
	while (spid->spi->perif->SR & SPI_SR_BSY);
}

static void prep_dma_8bit(struct dev_spi_dma_t* dma) {
	//disable DMA channel
	CLEAR_BIT(dma->ch->CCR, DMA_CCR_EN);

	MODIFY_REG(dma->ch->CCR,
		////////// clear bits //////////
		//clear memory size
		DMA_CCR_MSIZE_Msk |

		//clear peripheral size
		DMA_CCR_PSIZE_Msk |

		//disable peripheral increment mode
		DMA_CCR_PINC |

		//disable circular mode
		DMA_CCR_CIRC,

		////////// set bits //////////
		//enable memory increment mode
		DMA_CCR_MINC);
}

static void prep_dma_16bit(struct dev_spi_dma_t* dma) {
	//disable DMA channel
	CLEAR_BIT(dma->ch->CCR, DMA_CCR_EN);

	MODIFY_REG(dma->ch->CCR,
		////////// clear bits //////////
		//clear memory size
		DMA_CCR_MSIZE_Msk |

		//clear peripheral size
		DMA_CCR_PSIZE_Msk |

		//disable peripheral increment mode
		DMA_CCR_PINC |

		//disable circular mode
		DMA_CCR_CIRC,

		////////// set bits //////////
		//enable memory increment mode
		DMA_CCR_MINC |

		//set memory size to 16bit
		DMA_CCR_MSIZE_0 |

		//set peripheral size to 16bit
		DMA_CCR_PSIZE_0);
}

static void send(struct spi_t* spi, __IO uint8_t* data, size_t len) {
	//set data length
	spi->dma_tx.ch->CNDTR = len;

	//set memory pointer
	spi->dma_tx.ch->CMAR = (uint32_t)data;

	//set peripheral pointer
	spi->dma_tx.ch->CPAR = (uint32_t)&spi->perif->DR;

	SET_BIT(spi->dma_tx.ch->CCR,
		//transfer data from memory to peripheral
		DMA_CCR_DIR |

		//enable DMA
		DMA_CCR_EN);
}

static void send_circular(struct spi_t* spi, __IO uint8_t* data, size_t len) {
	//set data length
	spi->dma_tx.ch->CNDTR = len;

	//set memory pointer
	spi->dma_tx.ch->CMAR = (uint32_t)data;

	//set peripheral pointer
	spi->dma_tx.ch->CPAR = (uint32_t)&spi->perif->DR;

	MODIFY_REG(spi->dma_tx.ch->CCR,
		////////// clear bits //////////
		//disable memory increment mode
		DMA_CCR_MINC,

		////////// set bits //////////
		//transfer data from memory to peripheral
		DMA_CCR_DIR |

		//enable DMA
		DMA_CCR_EN);
}

static void recv(struct spi_t* spi, __IO uint8_t* data, size_t len) {
	//set data length
	spi->dma_rx.ch->CNDTR = len;

	//set memory pointer
	spi->dma_rx.ch->CMAR = (uint32_t)data;

	//set peripheral pointer
	spi->dma_rx.ch->CPAR = (uint32_t)&spi->perif->DR;

	SET_BIT(spi->dma_rx.ch->CCR,
		//enable DMA
		DMA_CCR_EN);
}

void spi_set8(struct spi_dev_t* spid) {
	//disable peripheral
	CLEAR_BIT(spid->spi->perif->CR1, SPI_CR1_SPE);

	//set data width to 8 bit & enable peripheral
	MODIFY_REG(spid->spi->perif->CR2, SPI_CR2_DS_Msk, (0b0111 << 8));

	//enable peripheral
	SET_BIT(spid->spi->perif->CR1, SPI_CR1_SPE);
}

void spi_set16(struct spi_dev_t* spid) {
	//disable peripheral
	CLEAR_BIT(spid->spi->perif->CR1, SPI_CR1_SPE);

	//set data width to 16 bit & enable peripheral
	MODIFY_REG(spid->spi->perif->CR2, SPI_CR2_DS_Msk, (0b1111 << 8));

	//enable peripheral
	SET_BIT(spid->spi->perif->CR1, SPI_CR1_SPE);
}


void spi_send8(struct spi_dev_t* spid, __IO uint8_t* data, size_t len) {
	prep_dma_8bit(&spid->spi->dma_tx);
	send(spid->spi, data, len);
}

void spi_sendCircular8(struct spi_dev_t* spid, __IO uint8_t* data, size_t len) {
	prep_dma_8bit(&spid->spi->dma_tx);
	send_circular(spid->spi, data, len);
}

void spi_recv8(struct spi_dev_t* spid, __IO uint8_t* data, size_t len) {
	prep_dma_8bit(&spid->spi->dma_rx);
	recv(spid->spi, data, len);
}


void spi_send16(struct spi_dev_t* spid, __IO uint16_t *data, size_t len) {
	prep_dma_16bit(&spid->spi->dma_tx);
	send(spid->spi, (__IO uint8_t*)data, len);
}

void spi_sendCircular16(struct spi_dev_t* spid, __IO uint16_t* data, size_t len) {
	prep_dma_16bit(&spid->spi->dma_tx);
	send_circular(spid->spi, (__IO uint8_t*)data, len);
}

void spi_recv16(struct spi_dev_t* spid, __IO uint16_t* data, size_t len) {
	prep_dma_16bit(&spid->spi->dma_rx);
	recv(spid->spi, (__IO uint8_t*)data, len);
}


void spi_xfer8(struct spi_dev_t* spid, __IO uint8_t* data_tx,
		__IO uint8_t* data_rx, size_t len) {

	prep_dma_8bit(&spid->spi->dma_rx);
	recv(spid->spi, data_rx, len);

	prep_dma_8bit(&spid->spi->dma_tx);
	send(spid->spi, data_tx, len);
}

void spi_xfer16(struct spi_dev_t* spid, __IO uint16_t* data_tx,
		__IO uint16_t* data_rx, size_t len) {

	prep_dma_16bit(&spid->spi->dma_rx);
	recv(spid->spi, (__IO uint8_t*)data_rx, len);

	prep_dma_16bit(&spid->spi->dma_tx);
	send(spid->spi, (__IO uint8_t*)data_tx, len);
}
