/*
 * parcour.c
 *
 * Created: 22/03/2023 11:02:27
 *  Author: joren
 */ 

#include "movementManagement.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

char richting;

Junctions situatie = Straight;

char parcour(char *route) {
	while(1) {
		situatie = lineType();
		
		switch (situatie) {
			case Barcode: // end of parcour
				set_motors(0,0);
				return 0;
			
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
				richting = 'R';
				strncat(route, &richting, 1);
				break;
			
			case Straight_right_junction:
				richting = 'S';
				strncat(route, &richting, 1);
			case Straight:
				motorControl('S');
				break;
			
			case Line_end:
				motorControl('T');
				richting = 'T';
				strncat(route, &richting, 1);
				break;
		}
		
		
		if (route[-2] == 'T') {
			route[strlen(route)-3] = '\0';
			if (route[-3] == 'L' && route[-1] == 'L') {
				richting = 'S';
			} else if (route[-3] == 'S' && route[-1] == 'L'){
				richting = 'R';			
			} else if (route[-3] == 'R' && route[-1] == 'L'){
				richting = 'T';			
			} else if (route[-3] == 'L' && route[-1] == 'R'){
				richting = 'T';			
			} else if (route[-3] == 'L' && route[-1] == 'S'){
				richting = 'R';			
			}
			strncat(route, &richting, 1);				
		}
	}
	return 0;
}