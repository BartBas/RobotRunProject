/*
 * parcour.c
 *
 * Created: 22/03/2023 11:02:27
 *  Author: joren
 */ 

#include "movementManagement.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

char route[20];

Junctions situatie = Straight;

void parcour(void) {
	while(1) {
		situatie = direction();
		
		switch (situatie) {
			case Barcode: // end of parcour
			set_motors(0,0);
			return;
			
			case X_junction:
			case Left_corner:
			case T_junction:
			case Straight_left_junction:
			motorcontrol('L');
			route += 'L';
			break;
			
			case Right_corner:
			motorcontrol('R');
			route += 'R';
			break;
			
			case Straight_right_junction:
			route += 'S';
			case Straight:
			motorcontrol('S');
			break;
			
			case Line_end:
			motorcontrol('T');
			route += 'T';
			break;
		}
		
		//if  route[-2]
	}
}