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
#include "keypad.h"

/* Private variables ---------------------------------------------------------*/
//sensitivity_component
float offset[4][3] = {{0.000947840438189 ,-0.000019936494628 ,-0.000023987290420},
  {-0.000022355958866 ,0.000964651608004 ,-0.000010465713571},
  {0.000004748462666 ,-0.000004871582500 , 0.001011092805443},
	{-0.006723511104368,-0.002245066070008,-0.018506478672045}
};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void configure(void);
float* multiplyMatrix(float* input);
int goal = 0;	 
	 
int main(void)
{		
	/* MCU Configuration----------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
	/* Initialize all configured peripherals */
	configure();
	//printf("* Configured * \n");
	
	while (1){
		//HAL_Delay(100);
	}
}

int input = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	float readings[3];
	float* calibrated_matrix;
	float output[3];
	
	LIS3DSH_ReadACC(output);
	//output = multiplyMatrix(output);
	//output = getNormalized(output);
	printFloatArray(getNormalized(multiplyMatrix(output)),3);
	
	if((goal<300) && input == 0){
		if (GPIO_PIN_6){
			columns();
			
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 0){
				printf("1");
				goal = 1 + (goal*10);
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_2) == 0){
				printf("2");
				goal = 2 + (goal*10)
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_3) == 0){
				printf("3");
				goal = 3 + (goal*10)
			}
			rows();
		}	
		else if (GPIO_PIN_7){
			columns();
			
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 0){
				printf("4");
				goal = 4 + (goal*10);
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_2) == 0){
				printf("5");
				goal = 5 + (goal*10)
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_3) == 0){
				printf("6");
				goal = 6 + (goal*10)
			}
			rows();
		}	
		else if (GPIO_PIN_8){
			columns();
			
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 0){
				printf("7");
				goal = 7 + (goal*10);
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_2) == 0){
				printf("8");
				goal = 8 + (goal*10)
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_3) == 0){
				printf("9");
				goal = 9 + (goal*10)
			}
			rows();
		}	
		else if (GPIO_PIN_9){
			columns();
			
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == 0){
				
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_2) == 0){
				printf("0");
				goal = 0 + (goal*10)
			}
			else if(HAL_GPIO_ReadPin(GPID, GPIO_PIN_3) == 0){
				input = 1;
			}
			rows();
		}	
}

	LIS3DSH_ReadACC(readings);
	calibrated_matrix = multiplyMatrix(readings);
	
	//printf("x -> %f, y -> %f, z -> %f \n",calibrated_matrix[0],calibrated_matrix[1],calibrated_matrix[2]);
	printFloatArray(readings,3);
}

float* multiplyMatrix(float* input){
	float raw_data[4] = {0};
	float calibrated_matrix[3] = {0};
	int i,j;
	
	raw_data[0] = input[0];
	raw_data[1] = input[1];
	raw_data[2] = input[2];
	raw_data[3] = 1;
	
	for (i=0; i<3; i++){
		for (j=0; j<4; j++){
			calibrated_matrix[i] += raw_data[j]*offset[j][i];
		}
	}
	
	return calibrated_matrix;
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

