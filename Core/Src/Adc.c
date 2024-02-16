/*
 * Adc.c
 *
 *  Created on: 3 Şub 2024
 *      Author: guven
 */
#include "Adc.h"
#include "timing.h"

uint32_t AdcData[4];
uint32_t refMsADC;
void RCC_Config() {
	RCC->CR |= 0x00010000;
	while (!(RCC->CR & 0x00020000))
		;
	RCC->CR |= 0x00080000;
	RCC->CR |= 0x01000000;
	RCC->PLLCFGR |= 0x00400000;
	RCC->PLLCFGR |= 0x00000004;
	RCC->PLLCFGR |= 0x00005A00;
	RCC->PLLCFGR |= 0x00000000;
	RCC->CFGR |= 0x00000000;
	RCC->CFGR |= 0x00080000;
	RCC->CFGR |= 0x00001400;
	RCC->CIR |= 0x00080000;
	RCC->CIR |= 0x00800000;
}

void GPIO_Config() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= 0x000000FF;
	GPIOC->OSPEEDR |= 0x000000FF;
}

void ADC_Config() {
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR1 |= (1 << 8);
	ADC1->CR1 |= (0 << 24);
	ADC1->CR2 |= (1 << 0);
	ADC1->CR2 |= (0 << 1); //Continuous Mode
	ADC1->CR2 |= (1 << 8);
	ADC1->CR2 |= (1 << 9);
	ADC1->CR2 |= (1 << 10); //EOC
	ADC1->CR2 |= (1 << 30);
	ADC1->SQR1 |= (3 << 20);
	ADC1->SQR3 |= (10 << 0);
	ADC1->SQR3 |= (11 << 5);
	ADC1->SQR3 |= (12 << 10);
	ADC1->SQR3 |= (13 << 15);
	refMsADC = TimingGetMs();
}

void DMA_Config() {
	RCC->AHB1ENR |= (1 << 22);
	while (DMA2_Stream4->CR & 0x00000001)
		;
	DMA2_Stream4->CR |= (0 << 6);
	DMA2_Stream4->CR |= (1 << 8);
	DMA2_Stream4->CR |= (0 << 9);
	DMA2_Stream4->CR |= (1 << 10);
	DMA2_Stream4->CR |= (2 << 11);
	DMA2_Stream4->CR |= (2 << 13);
	DMA2_Stream4->CR |= (3 << 16);
	DMA2_Stream4->CR |= (0 << 25);
	DMA2_Stream4->NDTR |= 4; //channel number;
	DMA2_Stream4->PAR |= (uint32_t) &ADC1->DR;
	DMA2_Stream4->M0AR |= (uint32_t) &AdcData;
	DMA2_Stream4->FCR |= (1 << 0);
	DMA2_Stream4->CR |= (1 << 0);
}

void AdcTask() {
	if (TimingCheckMs(refMsADC, 20)) {
		ADC1->CR2 |= ADC_CR2_SWSTART;
		while ((ADC1->SR & 0x00000002))
			;
		refMsADC = TimingGetMs();
	}
}

uint16_t GetIbusADCdata(int index)
{
	return ((AdcData[index]*1000)/4095)+1000;
}
