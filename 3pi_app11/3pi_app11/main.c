/*
 * parcour.c
 *
 * Created: 12-3-2023 12:05:39
 *  Author: Joost
 */ 

#include "warehouse.h"
#include "display.h"
#include "movementManagement.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

Junctions situatie = Straight;

void parcour() {
	while(1) {
		situatie = direction();
	
 		switch (situatie) {
			case Barcode: // end of parcour
				clear();
				lcd_goto_xy(0, 0);
				print("bar code");
				set_motors(0,0);
				delay(1000);
				break;
				
			case X_junction:
			case Left_corner:
			case T_junction:
			case Straight_left_junction:
				clear();
				lcd_goto_xy(0, 0);
				print("L");
		 		motorControl('L');
				clear();
		 		break;
					 
 			case Right_corner:
				clear();
				lcd_goto_xy(0, 0);
				print("R");
 				motorControl('R');
				clear();
				break;
				
			case Straight_right_junction:
			case Straight:
				clear();
				lcd_goto_xy(0, 0);
				print("S");
				motorControl('S');
				clear();
				break;
				
			case Line_end:
				clear();
				lcd_goto_xy(0, 0);
				print("End");
				motorControl('T');
				clear();
				break;
			
 		}
	}
}


int main()
{
	pololu_3pi_init(2000);
	initialize();
	parcour();
}