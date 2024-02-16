/*
 * timing.h
 *
 *  Created on: Feb 3, 2024
 *      Author: guven
 */

#ifndef INC_TIMING_H_
#define INC_TIMING_H_

#include "main.h"
#include "stm32f4xx_it.h"
extern TIM_HandleTypeDef htim2;

unsigned int TimingGetMs(void);

unsigned int TimingCheckMs(unsigned int ref,unsigned int msPassed);

#endif /* INC_TIMING_H_ */
