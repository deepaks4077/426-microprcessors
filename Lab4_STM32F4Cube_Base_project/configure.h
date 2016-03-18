#ifndef CONFIG_ACC_H
#define CONFIG_ACC_H

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "stm32f4xx_hal_tim.h"

/**
  * @brief  This function configures the Accelerometer, GPIO and TIM3
	* @retval [none]							
  */
void configure(void);

/**
  * @brief  This function configure GPIOE for the accelerometer
	* @retval [none]							
  */
void configGPIOE(void);

/**
  * @brief  This function configures the Accelerometer interrupt
	* @retval [none]							
  */
void configLISD3SH(void);

/**
  * @brief  This function configures the GPIOs for the 7-segment pins
	* @retval [none]							
  */
void configSegmentPins(void);

/**
  * @brief  This function configures TIM3
	* @retval [none]							
  */
void configTimer(void);

/**
  * @brief  This function configures TIM3
	* @retval [none]							
  */
void configADC(void);
#endif
