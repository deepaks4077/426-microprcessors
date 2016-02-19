/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program which showcases ADC and TIM3 under the new firmware
	                       The ADC
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
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
#define DECIMAL_PT								 								-1

/* Private variables -----------------------------------------------------------*/
extern ADC_HandleTypeDef ADC1_Handle;
extern GPIO_InitTypeDef GPIO_A;
extern GPIO_InitTypeDef GPIO_E;
extern GPIO_InitTypeDef GPIO_B;

/* Private function prototypes -------------------------------------------------*/
void SystemClock_Config	(void);
float Convert_Voltage_To_Temperature(uint32_t Voltage);
void Display_Digit_At_Pos(int pos, int digit);
void Reset_Display(void);
int Get_Digit_In_Place(float number, int place);

int main(void)
{
	uint32_t Voltage;
	float Temperature;
	int digit;
	int pos;
	int UPDATE_FLAG;
	int upd_ctr;
	int first_digit;
	int second_digit;
	int third_digit;
	int delay_btw_digits = 3;
	
	
	/* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	/* Configure and start the ADC1 peripheral (including the clock) -------------*/
	ADC_config();
	
	/* Configure the GPIO pins connected to the 7-segment display ----------------*/
	GPIO_config();
	
	UPDATE_FLAG = 1000/delay_btw_digits;
	upd_ctr = 0;
	
	while(1){
		Voltage = HAL_ADC_GetValue(&ADC1_Handle);
		Temperature = Convert_Voltage_To_Temperature(Voltage);
		
		printf("Temperature = %f \n", Temperature);
		first_digit = Get_Digit_In_Place(Temperature,1);
		second_digit = Get_Digit_In_Place(Temperature,10);
		third_digit = Get_Digit_In_Place(Temperature, 100);
		upd_ctr = 0;
		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
		
		printf("%d %d %d \n", first_digit, second_digit, third_digit);
		while(upd_ctr < UPDATE_FLAG){
			Display_Digit_At_Pos(1,third_digit);
			HAL_Delay(2);
			Reset_Display();
			
			Display_Digit_At_Pos(2,second_digit);
			Display_Digit_At_Pos(2,DECIMAL_PT);
			HAL_Delay(2);
			Reset_Display();
			
			Display_Digit_At_Pos(3,first_digit);
			HAL_Delay(2);
			Reset_Display();
			
			if(Temperature > 40){
				
			}
			
			upd_ctr++;
		}
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
	// RESEST ALL DIGITS AND CONTROLS
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
