/*
 * movementManagement.c
 *
 * Created: 22/03/2023 10:49:26
 *  Author: joren
 */ 
/*
 * movementManagement.c
 *
 * Created: 22/03/2023 10:49:26
 *  Author: joren
 */ 

unsigned int sensors[5]; // an array to hold sensor values
unsigned int position;
#define Refrence_value 400
#define turn_value 60
#define End_Line_Value 200

#include "movementManagement.h"
#include "Comunication.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

	Junctions junction;	
	int last_proportional;
	int integral;
			
void initialize()
{
	unsigned int counter; // used as a simple timer
	
	pololu_3pi_init(2000);
	
	while(!button_is_pressed(BUTTON_B)){
		clear();
		lcd_goto_xy(0,0);
		print_long(read_battery_millivolts());
	}	
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
	/*clear();
	lcd_goto_xy(0,0);
	print_long(x);	*/
}

void wait(){
	motorControl('N');
	
	while(!button_is_pressed(BUTTON_B)){}
	wait_for_button_release(BUTTON_B);
}

char lineType(){																																	// function that returns the type of junction it detects
	while(1){																																		// >= 500 = black line		<=500 = white
		read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
					
		if (sensors[0] >=Refrence_value && sensors[1] >= Refrence_value && sensors[4] <= Refrence_value ){																							//Checks if its a left corner or a straight with left corner
			inch();
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >= Refrence_value || sensors[3] >=Refrence_value){
				println(Straight_left_junction);
				return Straight_left_junction;
			}
			else{
				println(Left_corner);
				return Left_corner;
			}
		}
		
		
		else if (sensors[3] >=Refrence_value && sensors[4] >= Refrence_value && sensors[0] <= Refrence_value){																							//Checks if its a right corner or a straight with right corner
			inch();
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
			
			if (sensors[2] >= Refrence_value || sensors[1] >=Refrence_value){
				println(Straight_right_junction);
				return Straight_right_junction;
			}
			else{
				println(Straight_left_junction);
				return Right_corner;
			}
		}
		
				else if (sensors[1] >=Refrence_value && sensors[3] >= Refrence_value){																									//Checks if its a X junction or a T junction
					inch();
					read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
					
					if (sensors[2] >=Refrence_value){
						println(X_junction);
						return X_junction;
					}
					else {
						println(T_junction);
						return T_junction;
					}
				}
				
				else if(sensors[2] <= End_Line_Value && sensors[1] <=End_Line_Value && sensors[3] <=End_Line_Value){
					println(Line_end);																													// check if the line ends
					return Line_end;
				}
		else{
		//else if (sensors[2] >=Refrence_value){																													//Checks if the line is straight
			if ((sensors[3] <=Refrence_value && sensors[4] >=Refrence_value && sensors[1] <=Refrence_value) || (sensors[0] >=Refrence_value && sensors[1] <=Refrence_value && sensors[3] <=Refrence_value)){			//Checks the line for a "Barcode"
			//if((sensors[4] >=Refrence_value && sensors[0] >=Refrence_value) && (sensors[1] <=Refrence_value || sensors[3] <=Refrence_value)){
			
				while(sensors[4] >= Refrence_value || sensors[0] >=Refrence_value)
				{
					read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
					motorControl('S');
				}
				wait();
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
	unsigned int position = read_line(sensors,IR_EMITTERS_ON);
	read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
	
	if(x == 'N')															//Emergency Brake
		set_motors(0,0);	
	
	
	else if(x == 'L'){														//Turn Left
		set_motors(-turn_value,turn_value);
		
		while (sensors[2] >=Refrence_value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		while (sensors[2] <=Refrence_value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		set_motors(0,0);
	}
	
	
	else if(x == 'R'){														//Turn Right
		set_motors(turn_value,-turn_value);
		
		while (sensors[2] >=Refrence_value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		while (sensors[2] <=Refrence_value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		set_motors(0,0);
	}
		
			
	else if(x == 'S'){												//drive straight and adjusts the robot so it follows the line
		
		// are not interested in the individual sensor readings.
		// The "proportional" term should be 0 when we are on the line.
		int proportional = ((int)position) - 2000;

		// Compute the derivative (change) and integral (sum) of the
		// position.
		int derivative = proportional - last_proportional;
		integral = integral + proportional;

		// Remember the last position.
		 last_proportional = proportional;
 
 
		int power_difference = proportional/100 + integral/300 + derivative*0.10;						// derivative == stuur snelheid

		// Compute the actual motor settings.  We never set either motor
		// to a negative value.
		const int max = 70;
		if(power_difference > max)
			power_difference = max;
		if(power_difference < -max)
			power_difference = -max;

		if(power_difference < 0)
			set_motors(max+power_difference, max);
		else
			set_motors(max, max-power_difference);
		
	}
	
	
	else if(x == 'B'){														//Reverse while following the line
		set_motors(-50,-50);
		
		if (sensors[3] >=Refrence_value)
			set_motors(-50,-55);
		if (sensors[1] >=Refrence_value)
			set_motors(-55,-50);
	}
	
	
	else if(x == 'T'){														// Turn around
		set_motors(-turn_value,turn_value);
		
		while (sensors[2] <=Refrence_value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
	}
	
	
	else if(x == 'P')														//Robot stops on a slow pace
		for( int i=50;i>=0;i-=2)
		{
			set_motors(i,i);
			delay(5);
		}
		
		
}




void manualControl(Communications* X){
int speed = 0;

	if(X->Direction[0] == 1){
		set_motors(speed,speed);
		speed++;
	}
	
	if(X->Direction[1] == 1){
		set_motors(-speed,-speed);
		speed--;
	}
	
	if(X->Direction[2] == 1){
		set_motors(speed/2,speed);
	}
	
	if(X->Direction[3] == 1){
		set_motors(speed,speed/2);
	}
}

void Spin(Communications* X){
	
	for(int i = 0;i>=255;i++){
	set_motors(i,-i);	
	}
	while(X->EmergencyStop == 1){}
	for(int i = 255;i<=0;i--){
		set_motors(i,-i);
	}
}