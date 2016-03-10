#include "configure.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "stm32f4xx_hal_gpio.h"

ADC_HandleTypeDef ADC1_Handle;
GPIO_InitTypeDef GPIO_D;
GPIO_InitTypeDef GPIO_E;
TIM_HandleTypeDef TIM_type;

void configLISD3SH(void);
void configTimer(void);
void configGPIO(void);

void configure(void){
	configGPIO();
	configLISD3SH();
	configTimer();
}

void configLISD3SH(void){
		
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
	
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;          /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE ;                         /* Axes enable */
  LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; 			/* Block or update Low/High registers of data until all data is read */
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_50;												  	/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_2 ;                         /* Full scale 2 / 4 / 6 / 8 / 16 g */
  LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL ;

	LIS3DSH_IntConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;  
	LIS3DSH_IntConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_IntConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	//NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn,0,1);
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	LIS3DSH_Init(&LIS3DSH_InitStruct);
  LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);
}

void configGPIO(void){

	GPIO_InitTypeDef GPIO_E;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_E.Pin = GPIO_PIN_0;
	GPIO_E.Mode = GPIO_MODE_IT_FALLING;
	GPIO_E.Pull = GPIO_PULLDOWN;
	GPIO_E.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE,&GPIO_E);
}

void configTimer(void){
	
}

//intilize segment pins 
void GPIO_config(void){
	
	GPIO_D.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_D.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_D.Pull = GPIO_PULLUP;
	GPIO_D.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_E.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_E.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_E.Pull = GPIO_PULLUP;
	GPIO_E.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	HAL_GPIO_Init(GPIOD,&GPIO_D);
	HAL_GPIO_Init(GPIOE,&GPIO_E);
}

//intialize TIM timer 
void TimerTim(void)
{
	TIM_Base_InitTypeDef TIM_Time;
	
	TIM_Time.Period = 1;
	TIM_Time.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Time.Prescaler = 1;
	TIM_Time.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	TIM_Time.RepetitionCounter = 0;
	
	//initiate TIM3 
	TIM_type.Instance = TIM3;
	TIM_type.Init = TIM_Time; 
	TIM_type.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	TIM_type.Lock = HAL_UNLOCKED;
	TIM_type.State = HAL_TIM_STATE_READY;
	
	//Starting the clock 
	HAL_TIM_Base_MspInit(&TIM_type); 
	__TIM3_CLK_ENABLE(); 
	
	HAL_TIM_Base_Init(&TIM_type);	
	HAL_TIM_Base_Start_IT(&TIM_type);
	
	//Starting the interrupts
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 9, 9);

}