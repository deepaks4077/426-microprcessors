#include "kalman_filter.h"

float kalmanFilter(float Measurement, kalman_state* kstate){
	
	float tmp;

	kstate->p = kstate->p + kstate->q;
	tmp = (kstate->p + kstate->r);
	
	if(tmp != 0){
		kstate->k = kstate->p / (kstate->p + kstate->r);
	}else{
		return -1;
	}
	
	kstate->x = kstate->x + (kstate->k * (Measurement - kstate->x));
	kstate->p = (1 - kstate->k) * kstate->p;
	
	return kstate->x; 
}