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
#include "math.h"
#include "keypad.h"
#include "segments.h"
#include "kalman_filter.h"

/* Private variables ---------------------------------------------------------*/
//sensitivity_component
float offset[4][3] = {{0.000947840438189 ,-0.000019936494628 ,-0.000023987290420},
  {-0.000022355958866 ,0.000964651608004 ,-0.000010465713571},
  {0.000004748462666 ,-0.000004871582500 , 0.001011092805443},
	{-0.006723511104368,-0.002245066070008,-0.018506478672045}
};

float Pitch;
float Roll;

float first_digit, second_digit, third_digit;
int decimal_pos = -1;

int DISPLAY_CTR = 0; 														// Controls the rate at which a new value is displayed
int UPDATE_DIGIT_CTR = 0; 											// Controls the digit to display on the LED
float DISPLAY_ANGLE = 0.0;											// The angle to display	
int DISPLAY_DIRECTIONS_OR_ANGLE_FLAG = ANGLE;		// The mode of the application. Angle mode to display an angle and Directions mode

kalman_state kSx;
kalman_state kSy;
kalman_state kSz;

/*
*	The target angle
*/
int goal = 0;

/*
*	Controls keypad input mode
*/
int input = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

void configure(void);
/**
  * @brief  This function calculates the calibrated readings 
  * @param  [float] 		The acceleremoter raw readings
	* @retval [float]			The calibrated readings				
  */
float* multiplyMatrix(float* input);

/**
  * @brief  This function updates the digits that need to be displayed on the LED
  * @param  [float] 		The angle to get the LED digits from
	* @retval [none]					
  */
void getNewValue(float angle);

/**
  * @brief  This function handles the logic to display angles on the LED
  * @param  [void] 
	* @retval [none]					
  */
void displayAngle(void);

/**
  * @brief  This function handles the logic to display the directions to the goal angle
  * @param  [void] 		
	* @retval [none]					
  */
void displayDirections(void); 

/**
  * @brief  The handler for a button press
  * @param  [uint16_t]	The GPIO Pin which was pressed 		
	* @retval [none]					
  */
void buttonPressHandler(uint16_t GPIO_Pin);
	 
int main(void)
{		
	/* MCU Configuration----------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
	UPDATE_DIGIT_CTR = 0;
	
	kSx.k = 0.0;
	kSx.p = 1000;
	kSx.q = 5;
	kSx.r = 240;
	kSx.x = 0.0;

	kSy.k = 0.0;
	kSy.p = 1000;
	kSy.q = 5;
	kSy.r = 240;
	kSy.x = 0.0;

	kSz.k = 0.0;
	kSz.p = 1000;
	kSz.q = 5;
	kSz.r = 240;
	kSz.x = 1.0;
	
	/* Initialize all configured peripherals */
	configure();
	
	/* Initialize keypad to read rows first */
	rows();
	
	AddCelsius();
	while (1){
		//HAL_Delay(100);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	float readings[3];
	float* calibrated_matrix;
	float* kalman_output;
	float Ax, Ay,Az;
	
	if(GPIO_Pin == GPIO_PIN_0){
		LIS3DSH_ReadACC(readings);
	
		kalman_output = multiplyMatrix(readings);
	
		readings[0] = kalmanFilter(readings[0],&kSx); 
		readings[1] = kalmanFilter(readings[1],&kSy);
		readings[2] = kalmanFilter(readings[2],&kSz);
	
		calibrated_matrix = multiplyMatrix(readings);
		Ax = calibrated_matrix[0];
		Ay = calibrated_matrix[1];
		Az = calibrated_matrix[2];
	
		Pitch = atan2(Ax,Az) * 180 / 3.1415926515;
	}
	
	buttonPressHandler(GPIO_Pin);
}

/*
	Determines which button has been pressed, by first finding the row and then the column. 
*/
void buttonPressHandler(uint16_t GPIO_Pin){
	if((goal<180) && input == 0){
		if (GPIO_Pin == GPIO_PIN_6){ 
			// initiate the columns to set them as input to read the bit 			
			columns();																							
			// row one column 1
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
				printf("1");
				goal = 1 + (goal*10);
			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row one column 2
				printf("2");
				goal = 2 + (goal*10);
			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){ // row one column 3
				printf("3");
				goal = 3 + (goal*10);
			}
			// start the rows again for the next button press
			rows();												
		}	else if (GPIO_Pin == GPIO_PIN_7){
			columns();
			// row 2 column 1
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
				printf("4");
				goal = 4 + (goal*10);

			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row 2 column 2 
				printf("5");
				goal = 5 + (goal*10);

			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){ // row 2 column 3
				printf("6");
				goal = 6 + (goal*10);

			}
		
			rows();
		}	
		else if (GPIO_Pin == GPIO_PIN_8){
			columns();
			// row 3 column 1 
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
				printf("7");
				goal = 7 + (goal*10);

			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row 3 column 2
				printf("8");
				goal = 8 + (goal*10);

			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){ // row 3 column 3
				printf("9");
				goal = 9 + (goal*10);

			}

			rows();
		}	
		else if (GPIO_Pin == GPIO_PIN_9){
			columns();
			// row 4 column 1 
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0){
				goal = 0;

			}else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 0){ // row 4 column 2 
				printf("0");
				goal = 0 + (goal*10);

			}
			// row 4 column 3
			
			/*
				Once the hash button is presed, get out of this "if" statement and stop checking for button 
				presses and move to the direction function.
				
				Clear all interrupts and disable so that we dont have to check if a row is pressed again.
			*/	
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0){
				input = 1;
				printf("Changed mode, GOAL = %d\n", goal);
				DISPLAY_DIRECTIONS_OR_ANGLE_FLAG = DIRECTIONS;
				HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
				HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			}
			
			if(input == 0){
				// Added to handle a '0' input to the last row
				rows();
			}
		}	
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(DISPLAY_DIRECTIONS_OR_ANGLE_FLAG == ANGLE){
		if(DISPLAY_CTR % 200 == 0){
			getNewValue(Pitch);
		}
		displayAngle();
	}else if(DISPLAY_DIRECTIONS_OR_ANGLE_FLAG == DIRECTIONS){
		displayDirections();
	}else if(DISPLAY_DIRECTIONS_OR_ANGLE_FLAG == GOAL){
		if(DISPLAY_CTR % 200 == 0){
			getNewValue(goal);
		}
		displayAngle();
	}
}

void displayAngle(void){
	if(UPDATE_DIGIT_CTR == 0){
		
		if(DISPLAY_ANGLE< 10){
			decimal_pos = 1;
		}else if(DISPLAY_ANGLE < 100){
			decimal_pos = 2;
		}else{
			decimal_pos = 3;
		}
		
		Reset_Display();
		Display_Digit_At_Pos(1,third_digit);
		
		if(decimal_pos == 1){
			AddDecimal();
		}
		
		UPDATE_DIGIT_CTR++;
	}else if(UPDATE_DIGIT_CTR == 1){
		Reset_Display();
		Display_Digit_At_Pos(2,second_digit);
		
		if(decimal_pos == 2){
			AddDecimal();
		}
		
		UPDATE_DIGIT_CTR++;
	}else{
		Reset_Display();
		Display_Digit_At_Pos(3,first_digit);
		UPDATE_DIGIT_CTR = 0;
	}
}

/*
	Method depends on Pitch for user input.
	sets the lower and upperbounds according to the set goal.
	stay in this function as long as reset is not pressed. 
	If Pitch is not in range of goal show directins in which way to tilt.
*/
void displayDirections(void){
	
	float upper_bound = goal + 5.0;
	float lower_bound = goal - 5.0;
	
	Reset_Display();
	
	if(Pitch < lower_bound){ 
		// Counter-Clockwise
		Reset_Display();
		Display_Digit_At_Pos(1, 12);
	}else if(Pitch > upper_bound){ 
		// Clockwise
		Reset_Display();
		Display_Digit_At_Pos(1, 11);
	}else if(Pitch > lower_bound && Pitch < upper_bound){
		// in range display current pitch
		if(DISPLAY_CTR % 200 == 0){
			getNewValue(fabsf(Pitch));
		}
		displayAngle();
	}
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

void getNewValue(float angle){
	
	angle = fabsf(angle);
	if(angle < 10){
			first_digit = Get_Digit_In_Place(angle*10,1);
			second_digit = Get_Digit_In_Place(angle*10,10);
			third_digit = Get_Digit_In_Place(angle*10,100);
	}else if(angle < 100){
			first_digit = Get_Digit_In_Place(angle,1);
			second_digit = Get_Digit_In_Place(angle,10);
			third_digit = Get_Digit_In_Place(angle,100);
	}else{
			first_digit = Get_Digit_In_Place(angle/10.0,1);
			second_digit = Get_Digit_In_Place(angle/10.0,10);
			third_digit = Get_Digit_In_Place(angle/10.0,100);
	}
	
	DISPLAY_ANGLE = angle;
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

