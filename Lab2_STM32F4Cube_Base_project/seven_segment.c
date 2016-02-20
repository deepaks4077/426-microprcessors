#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "config.h"
#include "seven_segment.h"



void RESET_ALL_LEDS(void){
	HAL_GPIO_WritePin(GPIOD,ORANGE_LED,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GREEN_LED,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,BLUE_LED,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,RED_LED,GPIO_PIN_RESET);
}

void Light_Orange_Led(void){
	HAL_GPIO_WritePin(GPIOD,ORANGE_LED,GPIO_PIN_SET);
}
void Light_Green_Led(void){
	HAL_GPIO_WritePin(GPIOD,GREEN_LED,GPIO_PIN_SET);
}
void Light_Blue_Led(void){
	HAL_GPIO_WritePin(GPIOD,BLUE_LED,GPIO_PIN_SET);
}
void Light_Red_Led(void){
	HAL_GPIO_WritePin(GPIOD,RED_LED,GPIO_PIN_SET);
}


void Raise_The_Alarm(uint32_t led_ctr){
				
		if(led_ctr == 0){
			RESET_ALL_LEDS();
			Light_Orange_Led();
		}else if(led_ctr == UPDATE_ALARM){
			RESET_ALL_LEDS();
			Light_Green_Led();
		}else if(led_ctr == UPDATE_ALARM*2){
			RESET_ALL_LEDS();
			Light_Blue_Led();
		}else if(led_ctr == UPDATE_ALARM*3){
			RESET_ALL_LEDS();
			Light_Red_Led();
		}
}

int Get_Digit_In_Place(float number, int place)
{
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
