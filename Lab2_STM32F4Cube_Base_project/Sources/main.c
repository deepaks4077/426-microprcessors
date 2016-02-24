/**
  ******************************************************************************
  * File Name          	: main.c
  * Description        	: A program to diplsay temperature readings read from a 
  						 STMF324xx board on a 7-segment display
	* Author			: Deepak Sharma
	* Version           : 1.0.0
	* Date				: February 19th, 2016
  ******************************************************************************
  */
	
/* Includes --------------------------------------------------------------------*/
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
#define ALARM_THRESHOLD														31
#define UPDATE_TEMPERATURE										 	  10					

/* Private variables -----------------------------------------------------------*/
extern ADC_HandleTypeDef ADC1_Handle;
extern GPIO_InitTypeDef GPIO_A;
extern GPIO_InitTypeDef GPIO_E;
extern GPIO_InitTypeDef GPIO_B;
int UPDATE_FLAG;
int LED_CTR;
int UPDATE_ALARM;
typedef struct kalman_state{
	float q;
	float r;
	float x;
	float p;
	float k;
}kalman_state;


/* Private function prototypes -------------------------------------------------*/
void SystemClock_Config	(void);
float Convert_Voltage_To_Temperature(uint32_t Voltage);
void Display_Digit_At_Pos(int pos, int digit);
void Reset_Display(void);
int Get_Digit_In_Place(float number, int place);
void Raise_The_Alarm(uint32_t led_ctr);
void Light_Orange_Led(void);
void Light_Green_Led(void);
void Light_Blue_Led(void);
void Light_Red_Led(void);
void RESET_ALL_LEDS(void);
void Kalmanfilter(float Input,kalman_state* kstate);

int main(void)
{
	uint32_t Voltage;
	float Temperature;
	int first_digit;
	int second_digit;
	int third_digit;
	int alarm_ctr = 0;
	kalman_state kstate;
	FILE *ofp;
	char *mode = "rw+";
	
	kstate.q =	0.1;
	kstate.r =	0.1;	
	kstate.x =	0.0;
	kstate.p =	0.1;
	kstate.k =	0.0;
	
  /* MCU Configuration----------------------------------------------------------*/
 	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	/* Configure and start the ADC1 peripheral (including the clock) -------------*/
	ADC_config();
	
	/* Configure the GPIO pins connected to the 7-segment display ----------------*/
	GPIO_config();
	
	/* Variables which control the state of the display and alarm ----------------*/
	
	/* LED_CTR is updated every millisecond  by the SysTick handler --------------*/
	LED_CTR = 0;

	/* Used to activate the alarm ------------------------------------------------*/ 
	UPDATE_ALARM = UPDATE_TEMPERATURE/4;
	alarm_ctr = 0;
	
	while(1){
		Voltage = HAL_ADC_GetValue(&ADC1_Handle);
		Temperature = Convert_Voltage_To_Temperature(Voltage);
		Kalmanfilter(Temperature,&kstate);
		
		first_digit = Get_Digit_In_Place(Temperature,1);
		second_digit = Get_Digit_In_Place(Temperature,10);
		third_digit = Get_Digit_In_Place(Temperature, 100);
		
		if(Temperature > ALARM_THRESHOLD){
			UPDATE_ALARM = UPDATE_TEMPERATURE/4;
		}else{
			UPDATE_ALARM = 0;
		}
		
		LED_CTR = 0;
		alarm_ctr = 0;
		while(LED_CTR < UPDATE_TEMPERATURE){
			
			if(LED_CTR%3 == 0){
				Reset_Display();
				Display_Digit_At_Pos(1,third_digit);
			}else if(LED_CTR%3 == 1){
				Reset_Display();
				Display_Digit_At_Pos(2,second_digit);
				Display_Digit_At_Pos(2,DECIMAL_PT);
			}else if(LED_CTR%3 == 2){
				Reset_Display();
				Display_Digit_At_Pos(3,first_digit);
			}
						
			if(UPDATE_ALARM){
				Raise_The_Alarm(LED_CTR);
			}
			
			alarm_ctr++;
		}
		
		RESET_ALL_LEDS();
	}
}

void Kalmanfilter(float Input, kalman_state* kstate){
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	kstate->x = kstate->x + (kstate->k)*(Input - kstate->x);
	kstate->p = kstate->p - kstate->p * (kstate->k);
}

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

/* Convert the Voltage to Temperature ------------------------------------------*/
float Convert_Voltage_To_Temperature(uint32_t Voltage){
	
	float Temperature = Voltage * 1.0f;
	
	Temperature *= 3.30f/0xfff;
	Temperature -= 0.76;
	Temperature /= 0.025;
	Temperature += 25.0;
	
	return Temperature;
}

/* System Clock Configuration ------------------------------------------------- */
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts.*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
   * @brief A function used to setup the ADC to sample Channel 16
   * @retval None
   */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif
