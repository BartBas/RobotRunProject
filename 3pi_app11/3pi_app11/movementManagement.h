/*
 * movementManagement.h
 *
 * Created: 22/03/2023 10:49:53
 *  Author: joren
 */ 


#ifndef MOVEMENTMANAGEMENT_H_
#define MOVEMENTMANAGEMENT_H_

#include "Comunication.h"

typedef enum {					// enum for the corners and junctions
	Straight,					// ?
	Line_end,					//
	Barcode,					// ?
	
	Left_corner,				// ?
	Right_corner,				// ?
	
	Straight_right_junction,    // ?
	Straight_left_junction,		// ?
	T_junction,					// ?
	
	X_junction					// ?
	
} Junctions;


void motorControl(char x);
char lineType();
void inch();
void initialize(Communications* communications);
void display_readings(const unsigned int *calibrated_values);
void load_custom_characters();
void Spin();
void manualControl();



#endif /* MOVEMENTMANAGEMENT_H_ */