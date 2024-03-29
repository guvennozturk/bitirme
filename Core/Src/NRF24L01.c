/*
 ***************************************************************************************************************
 ***************************************************************************************************************
 ***************************************************************************************************************

 File:		  NRF24L01.c
 Author:     ControllersTech.com
 Updated:    30th APRIL 2021

 ***************************************************************************************************************
 Copyright (C) 2017 ControllersTech.com

 This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
 of the GNU General Public License version 3 as published by the Free Software Foundation.
 This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
 or indirectly by this software, read more about this on the GNU General Public License.

 ***************************************************************************************************************
 */

#include "stm32f4xx_hal.h"
#include "NRF24L01.h"

extern SPI_HandleTypeDef hspi1;
#define NRF24_SPI &hspi1

#define NRF24_CE_PORT   GPIOC
#define NRF24_CE_PIN    GPIO_PIN_4

#define NRF24_CSN_PORT   GPIOA
#define NRF24_CSN_PIN    GPIO_PIN_4

void CS_Select(void) {
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}

void CS_UnSelect(void) {
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

void CE_Enable(void) {
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
}

void CE_Disable(void) {
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
}

// write a single byte to the particular register
int nrf24_WriteReg(uint8_t Reg, uint8_t Data) {
	uint8_t buf[2];
	uint8_t ReceivedReg = 0;
	buf[0] = Reg | 1 << 5;
	buf[1] = Data;

	// Pull the CS Pin LOW to select the device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, buf, 2, 1000);
	CS_UnSelect();
	ReceivedReg=nrf24_ReadReg(Reg);
	// Pull the CS HIGH to release the device
	if (Reg == STATUS) {
		return 1;
	}
	if (ReceivedReg == Data) {
		return 1;
	} else {
		return 0;
	}
}

//write multiple bytes starting from a particular register
void nrf24_WriteRegMulti(uint8_t Reg, uint8_t *data, int size) {
	uint8_t buf[2];
	buf[0] = Reg | 1 << 5;
//	buf[1] = Data;

// Pull the CS Pin LOW to select the device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, buf, 1, 100);
	HAL_SPI_Transmit(NRF24_SPI, data, size, 1000);

	// Pull the CS HIGH to release the device
	CS_UnSelect();
}

uint8_t nrf24_ReadReg(uint8_t Reg) {
	uint8_t data = 0;

	// Pull the CS Pin LOW to select the device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, &data, 1, 100);

	// Pull the CS HIGH to release the device
	CS_UnSelect();

	return data;
}

/* Read multiple bytes from the register */
void nrf24_ReadReg_Multi(uint8_t Reg, uint8_t *data, int size) {
	// Pull the CS Pin LOW to select the device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, data, size, 1000);

	// Pull the CS HIGH to release the device
	CS_UnSelect();
}

// send the command to the NRF
uint8_t nrfsendCmd(uint8_t cmd) {
	// Pull the CS Pin LOW to select the device
	uint8_t data;
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 100);
	HAL_SPI_Receive(&hspi1, &data, 1,100);
	// Pull the CS HIGH to release the device
	CS_UnSelect();
	return data;
}

void nrf24_reset(uint8_t REG) {
	if (REG == STATUS) {
		nrf24_WriteReg(STATUS, 0x00);
	}

	else if (REG == FIFO_STATUS) {
		nrf24_WriteReg(FIFO_STATUS, 0x11);
	}

	else {
		nrf24_WriteReg(CONFIG, 0x08);
		nrf24_WriteReg(EN_AA, 0x3F);
		nrf24_WriteReg(EN_RXADDR, 0x03);
		nrf24_WriteReg(SETUP_AW, 0x03);
		nrf24_WriteReg(SETUP_RETR, 0x03);
		nrf24_WriteReg(RF_CH, 0x02);
		nrf24_WriteReg(RF_SETUP, 0x0E);
		nrf24_WriteReg(STATUS, 0x00);
		nrf24_WriteReg(OBSERVE_TX, 0x00);
		nrf24_WriteReg(CD, 0x00);
		uint8_t rx_addr_p0_def[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
		nrf24_WriteRegMulti(RX_ADDR_P0, rx_addr_p0_def, 5);
		uint8_t rx_addr_p1_def[5] = { 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 };
		nrf24_WriteRegMulti(RX_ADDR_P1, rx_addr_p1_def, 5);
		nrf24_WriteReg(RX_ADDR_P2, 0xC3);
		nrf24_WriteReg(RX_ADDR_P3, 0xC4);
		nrf24_WriteReg(RX_ADDR_P4, 0xC5);
		nrf24_WriteReg(RX_ADDR_P5, 0xC6);
		uint8_t tx_addr_def[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
		nrf24_WriteRegMulti(TX_ADDR, tx_addr_def, 5);
		nrf24_WriteReg(RX_PW_P0, 0);
		nrf24_WriteReg(RX_PW_P1, 0);
		nrf24_WriteReg(RX_PW_P2, 0);
		nrf24_WriteReg(RX_PW_P3, 0);
		nrf24_WriteReg(RX_PW_P4, 0);
		nrf24_WriteReg(RX_PW_P5, 0);
		nrf24_WriteReg(FIFO_STATUS, 0x11);
		nrf24_WriteReg(DYNPD, 0);
		nrf24_WriteReg(FEATURE, 0);
	}
}

void NRF24_Init(void) {
	// disable the chip before configuring the device
	CE_Disable();

	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config & 0x0D;
	uint8_t config2=nrf24_WriteReg(CONFIG, config);//First Power Down
	HAL_Delay(10);
	nrf24_WriteReg(RF_SETUP, 0x01);//Low Data Rate and Low Power(-18dsB)
	config2=nrf24_ReadReg(RF_SETUP);
	nrf24_WriteReg(SETUP_RETR,0x23);//Auto Retransmission delay of 750 microseconds and Up to 15 Retransmission on fail at AA
	config2=nrf24_ReadReg(SETUP_RETR);
	nrf24_WriteReg(FEATURE, 0x04);//Enable Dynamic Payload Length
	config2=nrf24_ReadReg(FEATURE);
	nrf24_WriteReg(DYNPD, 0x01);//Enable Dynamic Payload Length for data pipe 0.
	config2=nrf24_ReadReg(DYNPD);
	config2=nrf24_ReadReg(STATUS);

	// Enable the chip after configuring the device
	//CE_Enable();

}

// set up the Tx mode

void NRF24_TxMode(void) {
	//CE_Enable();
	// disable the chip before configuring the device
	nrf24_WriteReg(STATUS, 0x7E);//RX_DR,TX_DS,MAX_RT se 1 to clear.
	uint8_t config2=nrf24_ReadReg(STATUS);
	nrfsendCmd(FLUSH_RX);//Flush RX fifo.
	nrfsendCmd(FLUSH_TX);//Flush TX fifo
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config |2;//Power up and Tx Mode
	nrf24_WriteReg(CONFIG, config);
	HAL_Delay(10);
     config2=nrf24_ReadReg(CONFIG);
	CE_Enable();//İn order quit device from Standby1-Mode. We should do this.
}

// transmit the data
void nrf_rf_rx(unsigned short spi, char payload[]) {
	int payload_w = 0;
	int i;
	uint8_t r_rx_payload = R_RX_PAYLOAD;
	payload_w = nrf24_ReadReg(R_RX_PL_WID);
	CS_Select();
	HAL_SPI_Transmit(&hspi1, &r_rx_payload, 1,100);
	for (i = 0; i < payload_w; i++) {
		payload[i] = HAL_SPI_Receive(&hspi1, &r_rx_payload, 1, 100);
	}
	CS_UnSelect();
	uint8_t status = nrf24_ReadReg(STATUS);
	status = status | (1 << 6);
	nrf24_WriteReg(STATUS, status);
	nrfsendCmd(FLUSH_RX);
}

void nrf_rf_tx(unsigned short spi,unsigned char payload[])
{
	int i = 0;
	uint8_t w_tx_payload=W_TX_PAYLOAD;
	if(nrf24_ReadReg(CONFIG)&1)
	{
		NRF24_TxMode();
	}
	CS_Select();
	HAL_SPI_Transmit(&hspi1,&w_tx_payload,1,1000);
	//nrfsendCmd(W_TX_PAYLOAD);//Send Payload Command
	while(i<8)
	{
		HAL_SPI_Transmit(&hspi1,&payload[i] ,1, 100);//sending payload
		i++;
	}
	CS_UnSelect();

}
uint8_t NRF24_Transmit(uint8_t *data) {
	uint8_t cmdtosend = 0;

	// select the device
	CS_Select();

	// payload command
	cmdtosend = W_TX_PAYLOAD;
	HAL_SPI_Transmit(NRF24_SPI, &cmdtosend, 1, 100);

	// send the payload
	HAL_SPI_Transmit(NRF24_SPI, data, 32, 1000);

	// Unselect the device
	CS_UnSelect();

	HAL_Delay(1);

	uint8_t fifostatus = nrf24_ReadReg(FIFO_STATUS);

	// check the fourth bit of FIFO_STATUS to know if the TX fifo is empty
	if ((fifostatus & (1 << 4)) && (!(fifostatus & (1 << 3)))) {
		cmdtosend = FLUSH_TX;
		nrfsendCmd(cmdtosend);

		// reset FIFO_STATUS
		nrf24_reset(FIFO_STATUS);

		return 1;
	}

	return 0;
}

void NRF24_RxMode(void) {
	// disable the chip before configuring the device
	//CE_Disable();

	nrf24_WriteReg(STATUS, 0x7E);//RX_DR,TX_DS,MAX_RT se 1 to clear.
	nrfsendCmd(FLUSH_RX);//Flush RX fifo
	nrfsendCmd(FLUSH_TX);//Flush TX fifo
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config | ((1 << 1) | (1 << 0));//Power up and Rx Mode
	nrf24_WriteReg(CONFIG, config);
	HAL_Delay(10);

	CE_Enable();//İn order quit device from Standby1-Mode. We should do this.
}

uint8_t isDataAvailable(int pipenum) {
	uint8_t status = nrf24_ReadReg(STATUS);

	if ((status & (1 << 6)) && (status & (pipenum << 1))) {

		nrf24_WriteReg(STATUS, (1 << 6));

		return 1;
	}

	return 0;
}

void NRF24_Receive(uint8_t *data) {
	uint8_t cmdtosend = 0;

	// select the device
	CS_Select();

	// payload command
	cmdtosend = R_RX_PAYLOAD;
	HAL_SPI_Transmit(NRF24_SPI, &cmdtosend, 1, 100);

	// Receive the payload
	HAL_SPI_Receive(NRF24_SPI, data, 32, 1000);

	// Unselect the device
	CS_UnSelect();

	HAL_Delay(1);

	cmdtosend = FLUSH_RX;
	nrfsendCmd(cmdtosend);
}

// Read all the Register data
void NRF24_ReadAll(uint8_t *data) {
	for (int i = 0; i < 10; i++) {
		*(data + i) = nrf24_ReadReg(i);
	}

	nrf24_ReadReg_Multi(RX_ADDR_P0, (data + 10), 5);

	nrf24_ReadReg_Multi(RX_ADDR_P1, (data + 15), 5);

	*(data + 20) = nrf24_ReadReg(RX_ADDR_P2);
	*(data + 21) = nrf24_ReadReg(RX_ADDR_P3);
	*(data + 22) = nrf24_ReadReg(RX_ADDR_P4);
	*(data + 23) = nrf24_ReadReg(RX_ADDR_P5);

	nrf24_ReadReg_Multi(RX_ADDR_P0, (data + 24), 5);

	for (int i = 29; i < 38; i++) {
		*(data + i) = nrf24_ReadReg(i - 12);
	}

}

