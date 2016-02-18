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

/* Private variables -----------------------------------------------------------*/
extern ADC_HandleTypeDef ADC1_Handle;
extern GPIO_InitTypeDef GPIO_A;
extern GPIO_InitTypeDef GPIO_E;
extern GPIO_InitTypeDef GPIO_B;

/* Private function prototypes -------------------------------------------------*/
void SystemClock_Config	(void);
float Convert_Voltage_To_Temperature(uint32_t Voltage);

int main(void)
{
	uint32_t Voltage;
	float Temperature;
	
	/* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	/* Configure and start the ADC1 peripheral (including the clock) -------------*/
	ADC_config();
	
	/* Configure the GPIO pins connected to the 7-segment display ----------------*/
	GPIO_config();
	
	while(1){
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
	}
	
	//while (1){
		//Voltage = HAL_ADC_GetValue(&ADC1_Handle);
		//Temperature = Convert_Voltage_To_Temperature(Voltage);	
		//printf("TEMPERATURE %f <-------------------> VOLTAGE %d\n", Temperature, Voltage);
	//}
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
