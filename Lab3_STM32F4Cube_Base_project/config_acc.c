#include "config_acc.h"

void configure(){
	LIS3DSH_InitStruct->Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;         /* Ppower down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
  LIS3DSH_InitStruct->Axes_Enable = LIS3DSH_XYZ_ENABLE ;                        /* Axes enable */
  LIS3DSH_InitStruct->Continous_Update = LIS3DSH_ContinousUpdate_Enabled ;			/* Block or update Low/High registers of data until all data is read */
	LIS3DSH_InitStruct->AA_Filter_BW = LIS3DSH_AA_BW_50;													/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  LIS3DSH_InitStruct->Full_Scale = LIS3DSH_FULLSCALE_2 ;                        /* Full scale 2 / 4 / 6 / 8 / 16 g */
  LIS3DSH_InitStruct->Self_Test = LIS3DSH_SELFTEST_NORMAL ;   
}