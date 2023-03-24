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
void println(int x){
	clear();
	lcd_goto_xy(0,0);
	print_long(x);	
	
}


char lineType(){																																	// function that returns the type of junction it detects
	while(1){																																		// >= 500 = black line		<=500 = white
		read_line(sensors,IR_EMITTERS_ON);
		
		if (sensors[1] >=500 && sensors[3] >= 500){																									//Checks if its a X junction or a T junction
			inch();
			read_line(sensors,IR_EMITTERS_ON);
	
			if (sensors[2] >=500){
				println(X_junction);
				return X_junction;
			}
			else {
				println(T_junction);
				return T_junction;
			}
		}
			
			
		else if (sensors[1] >=500 && sensors[0] >= 500){																							//Checks if its a left corner or a straight with left corner
			inch();
			read_line(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >= 500 || sensors[3] >=500){
				println(Straight_left_junction);
				return Straight_left_junction;
			}
			else{
				println(Left_corner);
				return Left_corner;
			}
		}
		
		
		else if (sensors[3] >=500 && sensors[4] >= 500){																							//Checks if its a right corner or a straight with right corner
			inch();
			read_line(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >= 500 || sensors[1] >=500){
				println(Straight_right_junction);
				return Straight_right_junction;
			}
			else{
				println(Straight_left_junction);
				return Right_corner;
			}
		}
		
		
		else if(sensors[2] <=200){
			println(Line_end);																													// check if the line ends	
			return Line_end;
		}

		
		else if (sensors[2] >=500){																													//Checks if the line is straight
			if ((sensors[2] >=500 && sensors[3] <=500 && sensors[4] >=500) && (sensors[0] >=500 && sensors[1] <=500 && sensors[2] >=500))			//Checks the line for a "Barcode"
			{
				while(sensors[4] >= 400 || sensors[0] >=400)
				{
					read_line(sensors,IR_EMITTERS_ON);
					motorControl('S');
				}
				println(Barcode);
				return Barcode;
			}
			else{
				println(Straight);
				return Straight;
			}																									// check if its straight without any corners
			
		}	
	}
}


void motorControl(char x){													// function that controlls the motor movement and the turns
	read_line(sensors,IR_EMITTERS_ON);
	
	if(x == 'N')															//Emergency Brake
		set_motors(0,0);	
	
	
	else if(x == 'L'){														//Turn Left
		set_motors(-50,50);
		
		while (sensors[2] >=500)
			read_line(sensors,IR_EMITTERS_ON);
		while (sensors[2] <=500)
			read_line(sensors,IR_EMITTERS_ON);
	}
	
	
	else if(x == 'R'){														//Turn Right
		set_motors(50,-50);
		
		while (sensors[2] >=500)
			read_line(sensors,IR_EMITTERS_ON);
		while (sensors[2] <=500)
			read_line(sensors,IR_EMITTERS_ON);
	}
		
			
	else if(x == 'S'){														//drive straight and adjusts the robot so it follows the line
		set_motors(50,50);
		
		if (sensors[3] >= 500 && sensors[4] <= 300 )
			set_motors(50,30);
		if (sensors[1] >= 500 && sensors[0] <= 300 )
			set_motors(30,50);
	}
	
	
	else if(x == 'B'){														//Reverse while following the line
		set_motors(-50,-50);
		
		if (sensors[3] >=400)
			set_motors(-50,-55);
		if (sensors[1] >=400)
			set_motors(-55,-50);
	}
	
	
	else if(x == 'T'){														// Turn around
		set_motors(-75,75);
		
		while (sensors[2] >=500)
			read_line(sensors,IR_EMITTERS_ON);
	}
	
	
	else if(x == 'P')														//Robot stops on a slow pace
		for( int i=50;i>=0;i-=2)
		{
			set_motors(i,i);
			delay(5);
		}
		
		
}


void manualControl(char X){
int speed = 0;

	if(X == 'W'){
		set_motors(speed,speed);
		speed++;
	}
	
	else if(X == 'S'){
		set_motors(-speed,-speed);
		speed--;
	}
	
	else if(X == 'A'){
		set_motors(speed/2,speed);
	}
	
	else if(X == 'D'){
		set_motors(speed,speed/2);
	}
	
	if(X == 'R'){
		set_motors(255,-255);
	}
	
	
}