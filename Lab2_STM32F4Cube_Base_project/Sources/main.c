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
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handle;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void ADC_config(void);
ADC_ChannelConfTypeDef ADC_Get_ChannelConfig(void);
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
	
	/* Configure the ADC clock --------------------------------------------------*/
	__ADC1_CLK_ENABLE();
	
	/* Configure and start the ADC1 peripheral ---------------------------------------------*/
	ADC_config();
	
	while (1){
		Voltage = HAL_ADC_GetValue(&ADC1_Handle);
		Temperature = Convert_Voltage_To_Temperature(Voltage);	
		printf("TEMPERATURE %f <-------------------> VOLTAGE %d\n", Temperature, Voltage);
	}
}

/* Convert the Voltage to Temperature ------------------------------------------*/
float Convert_Voltage_To_Temperature(uint32_t Voltage){
	
	float Temperature = Voltage * 1.0f;
	
	Temperature *= 3000.0f/0xfff;
	Temperature /= 1000;
	Temperature -= 0.76;
	Temperature /= 0.025;
	Temperature += 25.0;
	
	return Temperature;
}


/** Configure and start ADC1 -------------------------------------------------------------*/
void ADC_config(void){

	ADC_InitTypeDef* ADC_Init;
	ADC_ChannelConfTypeDef* ADC1_Channel;
	
	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.State = HAL_ADC_STATE_READY;
	
	/* Configure init handler ----------------------------------------------------*/
	
	ADC_Init->ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC_Init->Resolution = ADC_RESOLUTION12b;
	ADC_Init->DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_Init->ScanConvMode = DISABLE;
	ADC_Init->EOCSelection = ADC_EOC_SINGLE_CONV; // | ADC_EOC_SEQ_CONV | ADC_EOC_SINGLE_SEQ_CONV
	ADC_Init->ContinuousConvMode = DISABLE;
	ADC_Init->NbrOfConversion = 1;
	ADC_Init->ExternalTrigConv = ADC_SOFTWARE_START;
	ADC_Init->ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	
	/* Add the init config handle to the ADC handler ----------------------------*/
	
	ADC1_Handle.Init = *ADC_Init;
	
	/* Configure the data acquisition channel -----------------------------------*/
	
	*ADC1_Channel = ADC_Get_ChannelConfig();
	
	HAL_ADC_ConfigChannel(&ADC1_Handle, ADC1_Channel);
	
	/* Initialize ADC1 -----------------------------------------------------------*/
	if(HAL_ADC_Init(&ADC1_Handle) != HAL_OK){
		Error_Handler(ADC_INIT_FAIL);
	}
	
	/* Start the ADC -------------------------------------------------------------*/
	HAL_ADC_Start(&ADC1_Handle);

}

/** Configure ADC1 Channel ---------------------------------------------------*/
ADC_ChannelConfTypeDef ADC_Get_ChannelConfig(){
	
	ADC_ChannelConfTypeDef* ADC1_Channel;
	
	ADC1_Channel->Channel = ADC_CHANNEL_16; // Grab the Temperature channel
	ADC1_Channel->Rank = 1;
	ADC1_Channel->Offset = 0;
	ADC1_Channel->SamplingTime = ADC_SAMPLETIME_480CYCLES;
	
	return *ADC1_Channel;
}



/** System Clock Configuration */
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
