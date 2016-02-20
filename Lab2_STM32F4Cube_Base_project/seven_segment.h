#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "config.h"

#define DECIMAL 									 ((uint16_t)0x4000)
#define SEG_A											 ((uint16_t)0x0080)
#define SEG_B	 		      	         ((uint16_t)0x0100)  
#define SEG_C     			           ((uint16_t)0x0200)  
#define SEG_D                			 ((uint16_t)0x0400)  
#define SEG_E             			   ((uint16_t)0x0800)  
#define SEG_F       			         ((uint16_t)0x1000)  
#define SEG_G 			               ((uint16_t)0x2000)
#define CTRL_1                		 ((uint16_t)0x0100)  
#define CTRL_2             			   ((uint16_t)0x0200)  
#define CTRL_3       			         ((uint16_t)0x0400)  
#define CTRL_4 			               ((uint16_t)0x0800)
#define GREEN_LED									 ((uint16_t)0x1000)
#define ORANGE_LED								 ((uint16_t)0x2000)
#define RED_LED										 ((uint16_t)0x4000)
#define BLUE_LED								   ((uint16_t)0x8000)
#define DECIMAL_PT								 								-1
#define ALARM_THRESHOLD													30.5
#define UPDATE_TEMPERATURE										 	1000	


