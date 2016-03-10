#ifndef CONFIG_ACC_H
#define CONFIG_ACC_H

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"

LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
GPIO_InitTypeDef GPIO_Struct;
TIM_HandleTypeDef TIM_Handle;
void configure(void);
void GPIO_config(void);
void TimerTim(void);
#endif