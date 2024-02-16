/*
 * timing.c
 *
 *  Created on: Feb 3, 2024
 *      Author: guven
 */
#include "timing.h"
unsigned int ms=0;
unsigned int ms2=0;
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  ms++;
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}


unsigned int TimingGetMs(void)
{


	return ms;
}


unsigned int TimingCheckMs(unsigned int ref,unsigned int msPassed)
{

   if((ms-ref)>msPassed)
   {
	   return 1;
   }

   else
	   {
	   return 0;
	   }

}
