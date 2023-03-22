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
char richting;

Junctions situatie = Straight;

void parcour(void) {
	while(1) {
		situatie = direction();
		
		switch (situatie) {
			case Barcode: // end of parcour
				set_motors(0,0);
				return route;
			
			case X_junction:
			case Left_corner:
			case T_junction:
			case Straight_left_junction:
				motorControl('L');
				richting = 'L';
				strncat(route, &richting, 1);
				break;
			
			case Right_corner:
				motorControl('R');
				richting = 'L';
				strncat(route, &richting, 1);
				break;
			
			case Straight_right_junction:
				richting = 'L';
				strncat(route, &richting, 1);
			case Straight:
				motorControl('S');
				break;
			
			case Line_end:
				motorControl('T');
				richting = 'L';
				strncat(route, &richting, 1);
				break;
		}
		
		//if  route[-2]
	}
}