#include "configure.h"


void LIS3DSH_GPIO_CONFIG(void);

void configure(void){
		
	LIS3DSH_InitStruct->Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;         /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  LIS3DSH_InitStruct->Axes_Enable = LIS3DSH_XYZ_ENABLE ;                        /* Axes enable */
  LIS3DSH_InitStruct->Continous_Update = LIS3DSH_ContinousUpdate_Enabled ;			/* Block or update Low/High registers of data until all data is read */
	LIS3DSH_InitStruct->AA_Filter_BW = LIS3DSH_AA_BW_50;													/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  LIS3DSH_InitStruct->Full_Scale = LIS3DSH_FULLSCALE_2 ;                        /* Full scale 2 / 4 / 6 / 8 / 16 g */
  LIS3DSH_InitStruct->Self_Test = LIS3DSH_SELFTEST_NORMAL ;

	
	LIS3DSH_Init(LIS3DSH_InitStruct);
}


void LIS3DSH_CPIO_CONFIG(void){
	GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the SPI periph */
  __SPI1_CLK_ENABLE();

  /* Enable SCK, MOSI and MISO GPIO clocks */
  __GPIOA_CLK_ENABLE();

  /* Enable CS, INT1, INT2  GPIO clock */
  __GPIOE_CLK_ENABLE();

  //GPIO_PinAFConfig(LIS3DSH_SPI_SCK_GPIO_PORT, LIS3DSH_SPI_SCK_SOURCE, LIS3DSH_SPI_SCK_AF);
  //GPIO_PinAFConfig(LIS3DSH_SPI_MISO_GPIO_PORT, LIS3DSH_SPI_MISO_SOURCE, LIS3DSH_SPI_MISO_AF);
  //GPIO_PinAFConfig(LIS3DSH_SPI_MOSI_GPIO_PORT, LIS3DSH_SPI_MOSI_SOURCE, LIS3DSH_SPI_MOSI_AF);

  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.Pin = LIS3DSH_SPI_SCK_PIN;
  HAL_GPIO_Init(LIS3DSH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.Pin =  LIS3DSH_SPI_MOSI_PIN;
  HAL_GPIO_Init(LIS3DSH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.Pin = LIS3DSH_SPI_MISO_PIN;
  HAL_GPIO_Init(LIS3DSH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin   = LIS3DSH_SPI_CS_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LIS3DSH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  HAL_GPIO_WritePin(LIS3DSH_SPI_CS_GPIO_PORT, LIS3DSH_SPI_CS_PIN, GPIO_PIN_SET);

  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin   = LIS3DSH_SPI_INT1_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(LIS3DSH_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = LIS3DSH_SPI_INT2_PIN;
  HAL_GPIO_Init(LIS3DSH_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
}