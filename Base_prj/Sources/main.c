#include <stdio.h>
//#include <demo.h> PLEASE MODIFY INCLUDE PATH FOR THE COMPILER
#include "arm_math.h"


/*
*	The Kalman filter state structure
*/
typedef struct kalman_state{
	float q;
	float r;
	float x;
	float p;
	float k;
}kalman_state;

void printArray(float* array, int length); 
void reverseArray(float* array, int length);
void subtract(float* input, float* output, int Length, float* sub);
float* calculate_stats(float* DifferenceArray, float* StatsArray, int Length);
void corr(float* input, float* output, float* corre, int Length);
void convolv(float* input, float* output, float* conv, int Length);
void testbench_asm(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);	
float* subtract_cmis(float* InputArray, float* OutputArray, float* DifferenceArray, int Length);
float* calculate_stats_cmis(float* DifferenceArray, float* StatsArray, int Length);
float* calculate_correlation_cmis(float* InputArray, float* OutputArray, float* CorrelationArray, int lengthInput, int lengthOutput);
float* calculate_convolution_cmis(float* InputArray, float* OutputArray, float* ConvolutionArray, int lengthInput, int lengthOutput);

int main()
{
	
	int Length = 5;
	float input[5]  = {-0.665365,-0.329988,0.164465,0.043962,0.295885};//{-1,0.125,31,1.0625};
	float output[5] = {0.0,0.0,0.0,0.0};
	float StatsArray[2] = {0.0,0.0};
	float DifferenceArray[5];
	float corre[9];
	float conv[9];
	
	//***SET STATE OF FILTER - SAME FOR DEMO*** //
	kalman_state kstate;
	
	//*********DEMO VALUES********************* //
	float OutputArrayDemo[5];
	float InputArrayDemo[5];
	float StatsArrayDemo[2];
	float DifferenceArrayDemo[5];
	float CorrelationArrayDemo[9];
	float ConvolutionArrayDemo[9];

	// INITIALIZE STATE VALUES //
	kstate.q =	0.1;
	kstate.r =	0.1;	
	kstate.x =	0.0;
	kstate.p =	0.1;
	kstate.k =	0.0;
	
	
	//************ASSEMBLY testbench************ //
	testbench_asm(input,output,&kstate,Length);
	printf("\nASSEMBLY TESTBENCH\n");
	printArray(output,Length);
	
	//************C filter********************** //
	printf("\nKALMAN FILTER\n");
	Kalmanfilter_C(input,output,&kstate,Length);
	
	//************ Subtraction****************** //
	printf("\nDIFFERENCE ARRAY\n");
	subtract(input,output,Length,DifferenceArray);
	printArray(DifferenceArray, Length);
	
	//************Mean and Standard Deviation*** //
	printf("\nMean and Standard Deviation\n");
	calculate_stats(DifferenceArray, StatsArray, Length);
	printArray(StatsArray, 2);
	
	//************Correlation Array************* //
	corr(input,output,corre,Length);
	printf("\nCORRELATION ARRAY\n");
	printArray(corre,2*Length-1);
	
	//************Convolution Array************* //
	convolv(input,output,conv,Length);
	printf("\nCONVOLUTION ARRAY\n");
	printArray(conv,2*Length-1);
	
	// REVERSE OUTPUT ARRAY FOR DSP FUNCTIONS    //
	reverseArray(output,Length);
	printf("\n <-------- CMIS-DSP Functions -------> \n");
	
	//************Difference Array***************//
	printf("\nDIFFERENCE ARRAY\n");
	subtract_cmis(input, output, DifferenceArray, Length);
	printArray(DifferenceArray, Length);
	
	//************Mean and Standard Deviation*** //
	printf("\nMean and Standard Deviation\n");
	calculate_stats_cmis(DifferenceArray, StatsArray, Length);
	printArray(StatsArray, 2);
	
	//************Correlation Array************* //
	calculate_correlation_cmis(input,output,corre,Length, Length);
	printf("\nCORRELATION ARRAY\n");
	printArray(corre,2*Length-1);
	
	//************Convolution Array************* //
	calculate_convolution_cmis(input,output,conv,Length, Length);
	printf("\nCONVOLUTION ARRAY\n");
	printArray(conv,2*Length-1);
	
	return 0;
}
	//********PRINTS OUT AN ARRAY************** //
void printArray(float* array, int length){  
	int i =0;
	for(i=0;i<length;i++){
		printf("%f \n", *(array+i));
	}
}
	//****REVERSES THE ARRAY FOR CONVOLUTION*** //
void reverseArray(float* array, int length){
	int i =0;
	float temp = 0.0;
	for(i=0;i<length/2;i++){
		temp = array[i];
		array[i] = array[length - i - 1];
		array[length - i - 1] = temp;
	}
}


/**
  * @brief  The Kalman filter function in C
  *         Takes in the input array, output array, filter state
  *         Updates the output array with filtered values.
  *
  * @param  InputArray 	-> The input array
  *			OutputArray -> The output array updated by this function
  *			kstate 		-> The kalman state struct
  *			Length 		-> The length of InputArray and OutputArray
  *
  * @retval 1 if error
  *			0 if successful
  */
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length){

	int i = 0;
	if(kstate->p == 0.0 && kstate->r == 0.0){
		return -1;
	}

	for(i=0;i<Length;i++){
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);

		//check for a NAN
		if(InputArray[i] != InputArray[i]){
			return -1;
		}

		//check for a NAN
		if(OutputArray[i] != OutputArray[i]){
			return -1;
		}	

		if(i==0){
			OutputArray[i] = 0 + kstate->k*(InputArray[i]);
		}else{
			OutputArray[i] = OutputArray[i-1] + kstate->k*(InputArray[i] - OutputArray[i-1]);
		}
		kstate->p = kstate->p - kstate->p * (kstate->k);
	}

	printArray(OutputArray,Length);

	return 0;
}

/**
  * @brief  Calculate the standard deviation and mean
  *         
  * @param  DifferenceArray	-> The input array
  *			StatsArray[0] 	-> The mean of the DifferenceArray 
  *			StatsArray[1] 	-> The standard deviation of the DifferenceArray		
  *			Length 			-> The length of the DifferenceArray
  *
  * @retval None
  */
float* calculate_stats(float* DifferenceArray, float* StatsArray, int Length){
	int i;
	float result;
	//CALCULATE MEAN //
	for(i=0;i<Length;i++){
		result = result + DifferenceArray[i];
	}
	
	StatsArray[0] = result/Length; 
	result = 0.0;
	// CALCULATE STANDARD DEV //
	for(i=0;i<Length;i++){
		result += pow((DifferenceArray[i] - StatsArray[0]), 2);
	}
	StatsArray[1]  = sqrt(result/(Length-1));
}

/**
  * @brief  Calculate the difference between input and output
  *         
  * @param  input	        -> The input array
  *			output       	-> The output array
  *			sub 	        -> The difference array updated by this function
  *			Length 			-> The length of sub, input and output
  *
  * @retval None
  */
void subtract(float* input, float* output, int Length, float* sub){
	int i;
	for(i=0;i<Length;i++){
		sub[i] = input[i] - output[i];
	}
}	

/**
  * @brief  Calculate the correlation array
  *         
  * @param  input	        -> The input array
  *			output       	-> The output array
  *			corre 	        -> The correlation array updated by this function
  *			Length 			-> The length of input and output
  *
  * @retval None
  */
void corr(float* input, float* output, float* corre, int Length){
	int i;
	int j;
	int k = 0;
	float sum = 0.0;
	for(i=-Length+1;i<Length;i++){
		for(j=0;j<Length;j++){
			if((j+i)<0){
				sum += 0;
			}else if((j+i)>=Length){
				sum += 0;
			}else{
				sum += input[i+j]*output[j];
			}
		}
		corre[k] = sum;
		sum = 0.0;
		k++;
	}
}

/**
  * @brief  Calculate the convolution array
  *         
  * @param  input	        -> The input array
  *			output       	-> The output array
  *			corre 	        -> The convolution array updated by this function
  *			Length 			-> The length of input and output
  *
  * @retval None
  */
void convolv(float* input, float* output, float* conv,int Length){
	int i, j;
	int k = 0;
	float sum = 0.0;
	
	// CONVOLUTION VECTOR IS INVERTED OVER THE ORIGIN //
	reverseArray(output,Length);
	for(i=-Length+1;i<Length;i++){
		for(j=0;j<Length;j++){
			if((j+i)<0){
				sum += 0;
			}else if((j+i)>=Length){
				sum += 0;
			}else{
				sum += output[j]*input[i+j];
			}
		}
		conv[k] = sum;
		sum = 0;
		k++;
	}
}

/**
  * @brief  The CMSIS-DSP implementation of subtraction
  *         
  * @param  InputArray	    -> The input array
  *			OutputArray     -> The output array
  *			DifferenceArray -> The Difference array updated by this function 
  *			Length 			-> The length of InputArray, OutputArray and DifferenceArray
  *
  * @retval DifferenceArray -> A pointer to the DifferenceArray
  */
float* subtract_cmis(float* InputArray, float* OutputArray, float* DifferenceArray, int Length){	
	arm_sub_f32(InputArray, OutputArray, DifferenceArray, Length);
	return DifferenceArray;
}

/*
 * @brief   The CMSIS-DSP function to calculate the mean and standard deviation of the difference array
 * 
 * @param   DifferenceArray-> The difference array of the input and output
 *			StatsArray[0]  -> mean
 * 			StatsArray[1]  -> standard deviation
 *			Length 		   -> array The length of the difference array
 */
float* calculate_stats_cmis(float* DifferenceArray, float* StatsArray, int Length){
  arm_mean_f32(DifferenceArray, Length, &StatsArray[0]);
	arm_std_f32(DifferenceArray, Length, &StatsArray[1]);
	
	return StatsArray;
}

/*
 * @brief   The CMSIS-DSP function to calculate correlation
 * 
 * @param   InputArray        -> The input array
 * 			OutputArray       -> The output array
 *			CorrelationArray  -> The correlation array updated by this function
 *			lengthInput       -> Length of the input array
 *			lengthOutput	  -> Length of the output array
 *
 * @retval	CorrelationArray  -> pointer to the correlation array
 */
float* calculate_correlation_cmis(float* InputArray, float* OutputArray, float* CorrelationArray, int lengthInput, int lengthOutput){
	
	arm_correlate_f32(InputArray, lengthInput, OutputArray, lengthOutput, CorrelationArray);
	return CorrelationArray;
}

/*
 * @brief   The CMSIS-DSP function to calculate convolution
 * 
 * @param   InputArray        -> The input array
 * 			OutputArray       -> The output array
 *			ConvolutionArray  -> The convolution array updated by this function
 *			lengthInput       -> Length of the input array
 *			lengthOutput	  -> Length of the output array
 *
 * @retval	ConvolutionArray  -> pointer to the convolution array
 */
float* calculate_convolution_cmis(float* InputArray, float* OutputArray, float* ConvolutionArray, int lengthInput, int lengthOutput){
	
	arm_conv_f32(InputArray, lengthInput, OutputArray, lengthOutput, ConvolutionArray);
	return ConvolutionArray;
}

