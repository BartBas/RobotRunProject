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
/*
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
*/

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
			motorControl('L');
			//route += 'L';
			break;
			
			case Right_corner:
			motorControl('R');
			//route += 'R';
			break;
			
			case Straight_right_junction:
			//route += 'S';
			case Straight:
			motorControl('S');
			break;
			
			case Line_end:
			motorControl('T');
			//route += 'T';
			break;
		}
		
		//if  route[-2]
	}
}