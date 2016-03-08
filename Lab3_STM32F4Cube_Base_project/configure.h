#ifndef CONFIG_ACC_H
#define CONFIG_ACC_H

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"

LIS3DSH_InitTypeDef *LIS3DSH_InitStruct;
void configure(void);

#endif