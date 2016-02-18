#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "config.h"

ADC_HandleTypeDef ADC1_Handle;
GPIO_InitTypeDef GPIO_A;
GPIO_InitTypeDef GPIO_E;
GPIO_InitTypeDef GPIO_B;

GPIO_InitTypeDef* GPIOxInit(uint32_t pinNum, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate);

/** Initialize configuration for a GPIO Port ---------------------------------------------*/
GPIO_InitTypeDef* GPIOxInit(uint32_t pinNum, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t alternate){
	
	GPIO_InitTypeDef* init;
	init->Pin = pinNum;
	init->Mode = mode;
	init->Pull = pull;
	init->Speed = speed;
	init->Alternate = alternate;

	return init;
}

/** Configure and start ADC1 -------------------------------------------------------------*/
void ADC_config(void){

	ADC_ChannelConfTypeDef ADC1_Channel;
	
	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.State = HAL_ADC_STATE_READY;
	
	/* Configure init handler ----------------------------------------------------*/
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
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

void GPIO_config(void){
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//GPIO_InitTypeDef* GPIO_A = GPIOxInit(GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF1_TIM1);
	//GPIO_InitTypeDef* GPIO_E = GPIOxInit(GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF1_TIM1);

	GPIO_A.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_7;
	GPIO_A.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_A.Pull = GPIO_PULLDOWN;
	GPIO_A.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_E.Pin = GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_15;
	GPIO_E.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_E.Pull = GPIO_PULLDOWN;
	GPIO_E.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	GPIO_B.Pin = GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_15;
	GPIO_B.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_B.Pull = GPIO_PULLDOWN;
	GPIO_B.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(GPIOA,&GPIO_A);
	HAL_GPIO_Init(GPIOE,&GPIO_E);
	HAL_GPIO_Init(GPIOB,&GPIO_B);
}


