#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

/*
*	The Kalman filter state structure
*/
typedef struct kalman{
	float q; 
	float r; 
	float x; 
	float p; 
	float k; 
}kalman_state;

/**
 * @brief A C implementation of a Kalman Filter 
 * @param Measurement
 * @param Kalman state as a C struct
 * @return Float
 **/
float kalmanFilter(float Measurement, kalman_state* kstate);

#endif