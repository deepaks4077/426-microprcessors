/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "stm32f4xx_hal_gpio.h"

/* Private variables ---------------------------------------------------------*/
float sensitivity_component[3][3] = {{0.000947840438189 ,-0.000019936494628 ,-0.000023987290420},
  {-0.000022355958866 ,0.000964651608004 ,-0.000010465713571},
   {0.000004748462666 ,-0.000004871582500 , 0.001011092805443}};

float zero_g_component[3];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void configure(void);
float* getNormalized(float* inputAcc);
float* multiplyMatrix(float* input);
	 
int main(void)
{	
	zero_g_component[0] = -0.006723511104368;
	zero_g_component[1] = -0.002245066070008;
	zero_g_component[2] = -0.018506478672045;
	  
	/* MCU Configuration----------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
	/* Initialize all configured peripherals */
	configure();
	printf("* Configured * \n");
	
 while (1){
		//HAL_Delay(100);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	float output[3];
	
	LIS3DSH_ReadACC(output);
	//output = multiplyMatrix(output);
	//output = getNormalized(output);
	printFloatArray(getNormalized(multiplyMatrix(output)),3);
}

float* getNormalized(float* inputAcc){
	int i = 0;
	
	printFloatArray(inputAcc,3);
	for(i=0;i<3;i++){
		inputAcc[i] = inputAcc[i] + zero_g_component[i];
	}
	
	return inputAcc;
}

float* multiplyMatrix(float* input){
	
	float output[3];
	float sum;
	int i = 0;
	int j = 0;
	
	printFloatArray(input,3);
	
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			sum += sensitivity_component[i][j]*input[j];
		}
		output[i] = sum;
		sum = 0;
	}
	
	return output;
}


/** System Clock Configuration*/
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
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

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

