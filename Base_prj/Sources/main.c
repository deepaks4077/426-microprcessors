#include <stdio.h>
#include "arm_math.h"

typedef struct kalman_state{
	float q;
	float r;
	float x;
	float p;
	float k;
}kalman_state;

void printArray(float* array, int length);
void reverseArray(float* array, int length);
void subtraction(float* output, float* input, int Length, float* sub);
float mean(float* sub, int Length);
float stad(float* sub, int Length, int mean);
void corr(float* input, float* output, float* corre, int Length);
void convolv(float* input, float* output, float* conv, int Length);
void testbench_asm(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);	

int main()
{
	int mean = 0;
	float std = 0.0;
	
	int Length = 4;
	float input[4]  = {-1,0.125,31,1.0625};
	float output[4] = {0.0,0.0,0.0,0.0};
	float output_asm[4] = {0.0,0.0,0.0,0.0};
	float* sub;
	float corre[7];
	float conv[7];

	int i = 0; 
	kalman_state kstate;
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
	
	//************Correlation Array************* //
	corr(input,output,corre,Length);
	printf("\nCORRELATION ARRAY\n");
	printArray(corre,2*Length-1);
	
	//************Convolution Array************* //
	convolv(input,output,conv,Length);
	printf("\nCONVOLUTION ARRAY\n");
	printArray(conv,2*Length-1);
	
	return 0;
}

void printArray(float* array, int length){
	int i =0;
	for(i=0;i<length;i++){
		printf("%f \n", *(array+i));
	}
}

void reverseArray(float* array, int length){
	int i =0;
	float temp = 0.0;
	for(i=0;i<length/2;i++){
		temp = array[i];
		array[i] = array[length - i - 1];
		array[length - i - 1] = temp;
	}
}

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

void subtraction(float* output, float* input, int Length, float* sub){
	int i;
	for(i=0;i<Length;i++){
		*(sub+i) = input[i] - output[i];
	}
}	

float mean(float* sub, int Length){
	int i;
	float result;
	for(i=0;i<Length;i++){
		result = result + *(sub+i);
	}
	return result/Length;
}


float stad(float* sub, int Length, int mean){
	int i;
	int x[4] = {0.0,0.0,0.0,0.0};
	float std = 0.0;
	int y = 0;
	for(i=0;i<Length;i++){
		x[i] = pow((*(sub+i) - mean), 2);
	}
  for (i=0;i<Length;i++){
		y = y + x[i];
	}
	std  = sqrt(y);
  return std;
}

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

void convolv(float* input, float* output, float* conv,int Length){
	int i, j;
	int k = 0;
	float sum = 0.0;
	
	reverseArray(output,Length);
	printf("\nINVERTED OUTPUT\n");
	printArray(output,Length);
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

