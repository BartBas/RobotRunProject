/*
 * movementManagement.c
 *
 * Created: 22/03/2023 10:49:26
 *  Author: joren
 */ 

unsigned int sensors[5]; // an array to hold sensor values
unsigned int position;

#include "movementManagement.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

	Junctions junction;	



void display_readings(const unsigned int *calibrated_values)
{
	unsigned char i;
	
	for(i=0;i<5;i++) {
		const char display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};		// Initialize the array of characters that we will use for the
		char c = display_characters[calibrated_values[i]/101];			    // The variable c will have values from 0 to 9, since
		print_character(c);
	}
}

void initialize()
{
	unsigned int counter; // used as a simple timer
	
	pololu_3pi_init(2000);
	
	while(!button_is_pressed(BUTTON_B)){}	
	wait_for_button_release(BUTTON_B);
	delay_ms(1000);

	for(counter=0;counter<80;counter++)
	{
		if(counter < 20 || counter >= 60)
		set_motors(40,-40);
		else
		set_motors(-40,40);
		calibrate_line_sensors(IR_EMITTERS_ON);
		
		delay_ms(20);
	}
	set_motors(0,0);
	
	while(!button_is_pressed(BUTTON_B)){}
	wait_for_button_release(BUTTON_B);
	
	clear();
}

void inch(){
	set_motors(50,50);
	delay(200);
	set_motors(0,0);
}

char lineType(){																											// function that returns the type of junction it detects
	while(1){																												// >= 500 = black line		<=500 = white
		read_line(sensors,IR_EMITTERS_ON);
		
		if (sensors[1] >=500 && sensors[3] >= 500){																			// this checks if its a X junction or a T junction
			inch();
			read_line(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >=500)
			return X_junction;
			else
			return T_junction;
		}
		
		else if (sensors[1] >=500 && sensors[0] >= 500){																// this function checks if its a left corner or a straight with left corner
			inch();
			read_line(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >= 500 || sensors[3] >=500)
			return Straight_left_junction;
			else
			return Left_corner;
		}
		
		else if (sensors[3] >=500 && sensors[4] >= 500){																	// this function checks if its a right corner or a straight with right corner
			inch();
			read_line(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >= 500 || sensors[1] >=500)
			return Straight_right_junction;
			else
			return Right_corner;
		}
		
		else if(sensors[2] <=200){																						// check if the line ends
			return Line_end;
		}

		
		else if (sensors[2] >=500){
			if ((sensors[2] >=500 && sensors[3] <=500 && sensors[4] >=500) && (sensors[0] >=500 && sensors[1] <=500 && sensors[2] >=500)){
				while(sensors[4] >= 400 || sensors[0] >=400){
					read_line(sensors,IR_EMITTERS_ON);
					motorControl('S');
				}		// checks if it detects the "barcode"
				return Barcode;
			}
			else
			return Straight;																						// check if its straight without any corners
			
		}
		
	}
}


void motorControl(char x){
	read_line(sensors,IR_EMITTERS_ON);
	
	if(x == 'N')
	set_motors(0,0);
	
	
	else if(x == 'L'){										//turn left
		set_motors(-50,50);
		while (sensors[2] >=500){
			read_line(sensors,IR_EMITTERS_ON);
		}
		while (sensors[2] <=500){
			read_line(sensors,IR_EMITTERS_ON);
		}
	}
	
	else if(x == 'R'){									//turn right
		set_motors(50,-50);
		while (sensors[2] >=500){
			read_line(sensors,IR_EMITTERS_ON);
		}
		while (sensors[2] <=500){
			read_line(sensors,IR_EMITTERS_ON);
		}
	}
	
	else if(x == 'S'){									//drive straight and corrects the adjusment of the robot if it aint straight
		set_motors(50,50);
		
		if (sensors[3] >=800)
		set_motors(100,40);
		if (sensors[1] >=800)
		set_motors(40,100);
	}
	
	else if(x == 'B'){									// reverse
		set_motors(-50,-50);
		if (sensors[3] >=400)
		set_motors(-50,-55);
		if (sensors[1] >=400)
		set_motors(-55,-50);
	}
	else if(x == 'T'){									// Turn around
		set_motors(-75,75);
		while (sensors[2] >=500){
			read_line(sensors,IR_EMITTERS_ON);
		}
	}
}