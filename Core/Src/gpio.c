/*
 * gpio.c
 *
 *  Created on: Feb 6, 2024
 *      Author: guven
 */

#include "gpio.h"
#include "timing.h"
uint16_t gpio_data[4];
uint32_t refMsGPIO;
void GpioInit() {
	refMsGPIO = TimingGetMs();
}
void GpioTask() {
	if (TimingCheckMs(refMsGPIO, 20)) {
		refMsGPIO = TimingGetMs();
		if ((!HAL_GPIO_ReadPin(SWITCH1_INPUT1_PORT, SWITCH1_INPUT1_PIN))
				&& (!HAL_GPIO_ReadPin(SWITCH1_INPUT2_PORT, SWITCH1_INPUT2_PIN))) {
			gpio_data[0] = 1000;

		} else if ((HAL_GPIO_ReadPin(SWITCH1_INPUT1_PORT, SWITCH1_INPUT1_PIN))
				&& (!HAL_GPIO_ReadPin(SWITCH1_INPUT2_PORT, SWITCH1_INPUT2_PIN))) {
			gpio_data[0] = 1500;
		}

		else if ((HAL_GPIO_ReadPin(SWITCH1_INPUT1_PORT, SWITCH1_INPUT1_PIN))
				&& (HAL_GPIO_ReadPin(SWITCH1_INPUT2_PORT, SWITCH1_INPUT2_PIN))) {
			gpio_data[0] = 2000;
		}
		if ((!HAL_GPIO_ReadPin(SWITCH2_INPUT1_PORT, SWITCH2_INPUT1_PIN))
				&& (!HAL_GPIO_ReadPin(SWITCH2_INPUT2_PORT, SWITCH2_INPUT2_PIN))) {
			gpio_data[1] = 1000;

		}
		if ((HAL_GPIO_ReadPin(SWITCH2_INPUT1_PORT, SWITCH2_INPUT1_PIN))
				&& (!HAL_GPIO_ReadPin(SWITCH2_INPUT2_PORT, SWITCH2_INPUT2_PIN))) {
			gpio_data[1] = 1500;
		}

		if ((HAL_GPIO_ReadPin(SWITCH2_INPUT1_PORT, SWITCH2_INPUT1_PIN))
				&& (HAL_GPIO_ReadPin(SWITCH2_INPUT2_PORT, SWITCH2_INPUT2_PIN))) {
			gpio_data[1] = 2000;
		}
	}
}
uint16_t GetIbusGPIOData(SwitchNumber switchNum) {

	return gpio_data[switchNum];
}
