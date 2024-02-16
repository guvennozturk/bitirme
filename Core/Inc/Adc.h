/*
 * Adc.h
 *
 *  Created on: 3 Şub 2024
 *      Author: guven
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stdint.h"
#include "main.h"
#include "stm32f4xx_it.h"

typedef enum{
	POT_0=0,
	POT_1,
	POT_2,
	POT_3,
}POT_NUMBER;

void RCC_Config();

void GPIO_Config();

void ADC_Config();

void DMA_Config();

void AdcTask();

uint16_t GetIbusADCdata(int index);
#endif /* INC_ADC_H_ */
