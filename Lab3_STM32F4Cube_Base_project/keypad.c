#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "keypad.h"

GPIO_InitTypeDef Rows;
GPIO_InitTypeDef Columns;
/*
ROW 1 --> GPIO_PIN_6
ROW 2 --> GPIO_PIN_7
ROW 3 --> GPIO_PIN_8
ROW 4 --> GPIO_PIN_9

COLUMN 1 --> GPIO_PIN_1
COLUMN 2 --> GPIO_PIN_2
COLUMN 3 --> GPIO_PIN_3
*/
void rows(void){
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	Rows.Pin = GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_7 | GPIO_PIN_6;
	Rows.Mode = GPIO_MODE_IT_FALLING;  														//the interrupt line where the row is detected first, since the initial but is 1 
																																//we have to look for a falling edge that occurrs when a button is pressed  
	Rows.Pull = GPIO_PULLUP;
	Rows.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC ,&Rows);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	Columns.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	Columns.Mode = GPIO_MODE_OUTPUT_PP;
	Columns.Pull = GPIO_NOPULL;
	Columns.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOD ,&Columns);
	
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn ,1,1);
	HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	
	
}

void columns(void){
	
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	Columns.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	Columns.Mode = GPIO_MODE_IT_FALLING;
	Columns.Pull = GPIO_PULLUP;
	Columns.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOD ,&Columns);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	Rows.Pin = GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_7 | GPIO_PIN_6;
	Rows.Mode = GPIO_MODE_OUTPUT_PP;
	Rows.Pull = GPIO_NOPULL;
	Rows.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOC ,&Rows);
	
	HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
	
}
