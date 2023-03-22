/*
 * Parcour.h
 *
 * Created: 22/03/2023 11:02:37
 *  Author: Joost
 */ 


#ifndef PARCOUR_H_
#define PARCOUR_H_

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


void parcour(void);

#endif /* PARCOUR_H_ */