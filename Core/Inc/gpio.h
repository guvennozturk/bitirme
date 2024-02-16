/*
 * gpio.h
 *
 *  Created on: Feb 6, 2024
 *      Author: guven
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_
#include "stm32f4xx.h"
#include "main.h"

#define SWITCH1_INPUT1_PORT GPIOA
#define SWITCH1_INPUT1_PIN  GPIO_PIN_0

#define SWITCH1_INPUT2_PORT GPIOA
#define SWITCH1_INPUT2_PIN  GPIO_PIN_1

#define SWITCH2_INPUT1_PORT GPIOA
#define SWITCH2_INPUT1_PIN  GPIO_PIN_2

#define SWITCH2_INPUT2_PORT GPIOA
#define SWITCH2_INPUT2_PIN  GPIO_PIN_3


typedef enum{
   SWITCH_0=0,
   SWITCH_1,
   SWITCH_2,
   SWITCH_3,
}SwitchNumber;

uint16_t GetIbusGPIOData(SwitchNumber switchNum);
void GpioTask(void);

void GpioInit(void);
#endif /* INC_GPIO_H_ */
