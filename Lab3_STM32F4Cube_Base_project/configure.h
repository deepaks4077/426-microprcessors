#ifndef CONFIG_ACC_H
#define CONFIG_ACC_H

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"

/**
  * @brief  This function configures the Accelerometer, GPIO and TIM3
	* @retval [none]							
  */
void configure(void);
#endif