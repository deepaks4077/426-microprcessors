#ifndef SEGMENTS_H
#define SEGMENTS_H

#define DECIMAL 									  GPIO_PIN_14
#define SEG_A											 	GPIO_PIN_7
#define SEG_B	 		      	         	GPIO_PIN_8  
#define SEG_C     			           	GPIO_PIN_9  
#define SEG_D                			 	GPIO_PIN_10
#define SEG_E             			   	GPIO_PIN_11
#define SEG_F       			         	GPIO_PIN_12
#define SEG_G											 	GPIO_PIN_13
#define CTRL_1                		 	GPIO_PIN_8  
#define CTRL_2             			   	GPIO_PIN_9
#define CTRL_3       			         	GPIO_PIN_10
#define CTRL_4 			               	GPIO_PIN_11
#define CELSIUS											GPIO_PIN_15
#define DECIMAL_PT								 					 -1
#define DIRECTIONS									1
#define ANGLE												0	

int Get_Digit_In_Place(float number, int place);

void Display_Digit_At_Pos(int pos, int digit);

void Reset_Display(void);

void ClearDecimal(void);

void AddDecimal(void);

void AddCelsius(void);

void ClearCelsius(void);

#endif