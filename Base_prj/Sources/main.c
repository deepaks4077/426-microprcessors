#include <stdio.h>
#include "arm_math.h"

typedef struct kalman_state{
	float q;
	float r;
	float x;
	float p;
	float k;
}kalman_state;

void testbench_asm(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);
void Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);	
int main()
{
	float input[4]  = {0.0,0.0,0.0,0.0};
	float output[4] = {0.0,0.0,0.0,0.0};
	int Length = 4;
	int i = 0; 
	int j=0;
	kalman_state kstate;
	kstate.q =	0.1;
	kstate.r =	0.1;	
	kstate.p =	0.0;
	kstate.x =	0.1;
	kstate.k =	0.0;
	testbench_asm(input,output,&kstate,Length);
	j = j+1;
	for(i=0;i<Length;i++){
		j = j+1;
		printf("%f, \n", output[i]);
	}
	return 0;
}
