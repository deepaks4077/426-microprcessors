#include "configure.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"

GPIO_InitTypeDef GPIO_D;
GPIO_InitTypeDef GPIO_E;
LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
TIM_HandleTypeDef TIM_3;
TIM_HandleTypeDef TIM_2;
ADC_HandleTypeDef ADC1_Handle;

void configureTemperatureTimer(void);
void configureSevenSegmentTimer(void);

void configure(void){
	configGPIOE();
	configLISD3SH();
	configSegmentPins();
	configTimer();
	configADC();
}

void configLISD3SH(void){
		
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
	
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;          /* Power down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE ;                         /* Axes enable */
  LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; 			/* Block or update Low/High registers of data until all data is read */
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_50;												  	/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_2;                         	/* Full scale 2 / 4 / 6 / 8 / 16 g */
  LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;

	LIS3DSH_IntConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;  
	LIS3DSH_IntConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_IntConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn,0,1);
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	LIS3DSH_Init(&LIS3DSH_InitStruct);
  LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);
}

void configGPIOE(void){

	GPIO_InitTypeDef GPIO_E;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_E.Pin = GPIO_PIN_0;
	GPIO_E.Mode = GPIO_MODE_IT_FALLING;
	GPIO_E.Pull = GPIO_PULLDOWN;
	GPIO_E.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE,&GPIO_E);
}

//initialize segment pins 
void configSegmentPins(void){
	
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

void configTimer(void)
{
	configureSevenSegmentTimer();
	configureTemperatureTimer();
}

void configureTemperatureTimer(void){
	TIM_Base_InitTypeDef TIM_Time;
	
	TIM_Time.Period = 1000;
	TIM_Time.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Time.Prescaler = 21000;
	TIM_Time.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Time.RepetitionCounter = 0;
	
	TIM_2.Instance = TIM2;
	TIM_2.Init = TIM_Time; 
	TIM_2.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	TIM_2.Lock = HAL_UNLOCKED;
	TIM_2.State = HAL_TIM_STATE_READY;
	
	__TIM2_CLK_ENABLE(); 
	
	HAL_TIM_Base_Init(&TIM_2);	
	HAL_TIM_Base_Start_IT(&TIM_2);
	
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 8, 9);
}

void configureSevenSegmentTimer(void){
	TIM_Base_InitTypeDef TIM_Time;
	
	TIM_Time.Period = 10;
	TIM_Time.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Time.Prescaler = 21000;
	TIM_Time.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Time.RepetitionCounter = 0;
	
	TIM_3.Instance = TIM3;
	TIM_3.Init = TIM_Time; 
	TIM_3.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	TIM_3.Lock = HAL_UNLOCKED;
	TIM_3.State = HAL_TIM_STATE_READY;
	
	__TIM3_CLK_ENABLE(); 
	
	HAL_TIM_Base_Init(&TIM_3);	
	HAL_TIM_Base_Start_IT(&TIM_3);
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 9, 9);
}

/** Configure and start ADC1 -------------------------------------------------------------*/
void configADC(void){

	ADC_ChannelConfTypeDef ADC1_Channel;
	
	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.State = HAL_ADC_STATE_READY;
	
	/* Configure init handler ----------------------------------------------------*/
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC1_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_Handle.Init.ScanConvMode = DISABLE;
	ADC1_Handle.Init.EOCSelection = DISABLE;
	ADC1_Handle.Init.ContinuousConvMode = ENABLE;
	ADC1_Handle.Init.DMAContinuousRequests = DISABLE;
	ADC1_Handle.Init.NbrOfConversion = 1;
	ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC1_Handle.Init.NbrOfDiscConversion = 0;
	ADC1_Handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	
	/* Configure the data acquisition channel ------------------------------------*/
	ADC1_Channel.Channel = ADC_CHANNEL_16;
	ADC1_Channel.Rank = 1;
	ADC1_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC1_Channel.Offset = 0;

	/* Enable the ADC clock ------------------------------------------------------*/
	__ADC1_CLK_ENABLE();
	
	/* Initialize ADC1 -----------------------------------------------------------*/
	if(HAL_ADC_Init(&ADC1_Handle) != HAL_OK){
		Error_Handler(ADC_INIT_FAIL);
	}

	/* Configure the channel -> MUST BE DONE AFTER STARTING THE CLOCK ------------*/
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_Channel);
	
	/* Start the ADC -------------------------------------------------------------*/
	HAL_ADC_Start(&ADC1_Handle);
}