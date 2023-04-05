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
#include "display.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

	Junctions junction;	

	Communications *myComs;
	
			int last_proportional;
			int integral;	
	
void initialize(Communications* communications)
{
	myComs = communications;
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
	set_motors(0,0);
						
	while(!button_is_pressed(BUTTON_B)){}
	wait_for_button_release(BUTTON_B);
}

char lineType(){																																	// function that returns the type of junction it detects
	while(1){																																		// >= 500 = black line		<=500 = white
		read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);

			if (sensors[0] >= Refrence_value && sensors[4] >= Refrence_value){																									//Checks if its a X junction or a T junction
					inch();
					read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
					if (sensors[0] >=Refrence_value && sensors[1] >=Refrence_value && sensors[3] >= Refrence_value && sensors[4] >=Refrence_value){
						clear();
						while(sensors[0] >= 400 || sensors[4] >=400)
						{
							read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
							set_motors(75,75);
						}
						inch();
						wait();
						return Barcode;
					}
					else if (sensors[2] >=Refrence_value || (sensors[1] >= Refrence_value || sensors[3] >= Refrence_value)){
						println(X_junction);
						return X_junction;
					}
					else {
						println(T_junction);
						return T_junction;
					}
				}
							
		else if (sensors[0] >=Refrence_value && sensors[1] >= Refrence_value && sensors[4] <= Refrence_value ){																							//Checks if its a left corner or a straight with left corner
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
		
				else if(sensors[2] <= End_Line_Value && sensors[1] <=End_Line_Value && sensors[3] <=End_Line_Value){
					println(Line_end);																													// check if the line ends
					return Line_end;
				}
		else{
		//else if (sensors[2] >=Refrence_value){																													//Checks if the line is straight
			//if (((sensors[1] <=Barcode_White || sensors[3] <=Barcode_White) && sensors[4] >=Barcode_Black) || ((sensors[1] <=Barcode_White || sensors[3] <=Barcode_White) && sensors[0] >=Barcode_Black)){			//Checks the line for a "Barcode"
			/*if(sensors[4] <=Barcode_Black && sensors[3] >=Barcode_Black && sensors[1] >=Barcode_Black && sensors[0] <=Barcode_Black){
				clear();
				while(sensors[1] >= 400 || sensors[3] >=400)
				{
						red_led(LOW);				
											lcd_goto_xy(0,0);
											print_long(sensors[1]);
											lcd_goto_xy(0,1);
											print_long(sensors[3]);	
												lcd_goto_xy(5,0);
												print_long(sensors[0]);
												lcd_goto_xy(5,1);
												print_long(sensors[4]);									
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
			}				*/																					// check if its straight without any corners
			return Straight;
		}
		
	
	}
}

void motorControl(char x){													// function that controlls the motor movement and the turns

		


	myComs->Update(myComs);
	unsigned int position = read_line(sensors,IR_EMITTERS_ON);
	read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);

	while (analog_read(5) >= 200) {
		set_motors(0, 0);
		errorDisplay(object, batteryPercentage());
	}
	
	if(myComs->EmergencyStop == 1)		{													//Emergency Brake
		set_motors(0,0);
		errorDisplay(emergency,batteryPercentage());
	}else if(x == 'L'){														//Turn Left
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
 
 
		int power_difference = proportional/10 + integral/40000 + derivative*0.50;						// derivative == stuur snelheid

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
		for( int i=50;i>=0;i-=3)
		{
			set_motors(i,i);
			delay(4);
		}
		
		
}



void manualControl(){
static int speed = 0;
static unsigned int counter = 0;

updateDisplay(0,batteryPercentage(),manual);
	myComs->Update(myComs);
	if(myComs->Direction[0] == 1){
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed++;
		}
		if(speed > 25)
		{
			speed = 25;
		}
		counter++;
	}
	else if(myComs->Direction[0] == 0 && myComs->Direction[2] == 0)
	{
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed--;
		}
		if(speed < 1)
		{
			speed = 0;
		}
		counter++;
	}
	
	else if(myComs->Direction[2] == 1){
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed--;
		}
		if(speed < -25)
		{
			speed = -25;
		}
		counter++;
	}
	else if(myComs->Direction[0] == 1 && myComs->Direction[2] == 0)
	{
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed++;
		}
		if(speed > -1)
		{
			speed = 0;
		}
		counter++;
	}
	
	if(myComs->Direction[1] == 1){
		set_motors(speed/2,speed);
	}
	
	if(myComs->Direction[3] == 1){
		set_motors(speed,speed/2);
	}
}

void Spin(){
	errorDisplay(slipped,batteryPercentage());
	for(int i = 0;i<=255;i++){
		set_motors(i,0-i);
		delay(2);
	}
	int count = 0;
	while(myComs->EmergencyStop == 2){
		count++;
		if(count % 10000 == 0)
		{
			myComs->Update(myComs);
		}
		


	}
	
	set_motors(0,0);

	
}