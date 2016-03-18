#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "segments.h"


int Get_Digit_In_Place(float number, int place){
	int digit = (int) (number * 10);
	digit = (int) (digit/place) % 10;
	return digit;
}

void Display_Digit_At_Pos(int pos, int digit){

	switch(pos){
		case 1:
			HAL_GPIO_WritePin(GPIOD,CTRL_1,GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOD,CTRL_2,GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOD,CTRL_3,GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOD,CTRL_4,GPIO_PIN_SET);
			break;
	}
	
	switch(digit){
		case DECIMAL_PT:
			HAL_GPIO_WritePin(GPIOE,DECIMAL,GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_E,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_E,GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			break;
		case 8:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_E,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_SET);
			break;
		case 0:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_E,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			break;
		case 11:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_E,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			break;
		case 12:
			HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_SET);
			break;
	
	}
}

void Reset_Display(void){
	// RESET ALL DIGITS AND CONTROLS
		HAL_GPIO_WritePin(GPIOE,DECIMAL,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_A,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_B,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_C,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_D,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_E,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_F,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,SEG_G,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,CTRL_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,CTRL_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,CTRL_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,CTRL_4,GPIO_PIN_RESET);
}


void ClearDecimal(void){
	HAL_GPIO_WritePin(GPIOE,DECIMAL,GPIO_PIN_RESET);
}

void AddDecimal(void){
	HAL_GPIO_WritePin(GPIOE,DECIMAL,GPIO_PIN_SET);
}

void AddCelsius(void){
	HAL_GPIO_WritePin(GPIOE,CELSIUS,GPIO_PIN_SET);
}

void ClearCelsius(void){
	HAL_GPIO_WritePin(GPIOE,CELSIUS,GPIO_PIN_RESET);
}
