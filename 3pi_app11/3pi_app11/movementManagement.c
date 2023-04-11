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

unsigned int sensors[5];			// an array to hold sensor values
unsigned int position;
#define Reference_Lower_Value	200
#define Reference_Upper_Value	400
#define turn_value 60
#define End_Line_Value 300


#include "movementManagement.h"
#include "Comunication.h"
#include "display.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

Junctions junction;
Communications *myComs;
	
int last_proportional;				// values for the PID Controller
int integral;
	
void initialize(Communications* communications)								// initialize for sensors
{
	myComs = communications;
	unsigned int counter;			 // used as a simple timer
	
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

void inch(){							//drives the robot a bit forward
	set_motors(50,50);
	delay(200);
	set_motors(0,0);
}

void wait(){							// wait function for the robot so it will wait for a button press B
	set_motors(0,0);
						
	while(!button_is_pressed(BUTTON_B)){}
		wait_for_button_release(BUTTON_B);
}

char lineType(){																																								// function that returns the type of junction it detects																																								// >= black line		<=white
	read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
	
	if (sensors[1] >= Reference_Upper_Value && sensors[3] >= Reference_Upper_Value){																									//Checks if its A "Barcode", a X junction or a T junction and returns the junction
		inch();
		read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		
		if (sensors[0] > Reference_Upper_Value && sensors[1] > Reference_Upper_Value && sensors[3] > Reference_Upper_Value && sensors[4] > Reference_Upper_Value){
			set_motors(75,75);
			while(sensors[0] > Reference_Upper_Value || sensors[4]  > Reference_Upper_Value)
			{
				read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
			}
			inch();
			delay(1000);
			return Barcode;
		}
		
		else if (sensors[2] > Reference_Upper_Value || (sensors[1] > Reference_Upper_Value || sensors[3]  > Reference_Upper_Value)){
			return X_junction;
		}
		else if(sensors[2] < Reference_Lower_Value){
			return T_junction;
		}
		
	}
	
							
	else if (sensors[0] > Reference_Upper_Value && sensors[1] > Reference_Upper_Value && sensors[4] < 350 ){																	//Checks if its a left corner or a straight with left corner and returns the junction
		inch();
		read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
			
		if (sensors[2] > 300 || sensors[3]  > 300){
			return Straight_left_junction;
		}
		else {
			return Left_corner;
		}
	}
		
		
	else if (sensors[3] > Reference_Upper_Value && sensors[4] > Reference_Upper_Value && sensors[0] < Reference_Lower_Value){																	//Checks if its a right corner or a straight with right corner and returns the junction
		inch();
		read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
			
		if (sensors[2] > Reference_Upper_Value || sensors[1]  > Reference_Upper_Value){
			return Straight_right_junction;
		}
		else {
			return Right_corner;
		}
	}
	
	else if(sensors[0] < End_Line_Value && sensors[1] < End_Line_Value && sensors[2] < End_Line_Value &&sensors[3] < End_Line_Value &&sensors[4] < End_Line_Value){															// check if the line ends and returns the junction																												
		return Line_end;
	}
	
																																									// else its a straight
	return Straight;
	
}

void motorControl(char x){																																					// function that controls the motor movement and the turns
	myComs->Update(myComs);
	unsigned int position = read_line(sensors,IR_EMITTERS_ON);
	read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
	
	while (analog_read(7) >= 200) {																																			// Checks if there is an object infront of it, if there is it stops the motor and gives feedback to the display
		set_motors(0, 0);
		errorDisplay(object, batteryPercentage());
	}

		
	if(myComs->EmergencyStop == 1){																																	//Emergency Brake
		set_motors(0,0);
		errorDisplay(emergency,batteryPercentage());	
	}
	
	
	else if(x == 'L'){																																						//Turn Left
		set_motors(-turn_value,turn_value);
		
		while (sensors[2] > Reference_Lower_Value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		while (sensors[2] < Reference_Upper_Value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		set_motors(0,0);
	}
	
	
	else if(x == 'R'){																																						//Turn Right
		set_motors(turn_value,-turn_value);
		
		while (sensors[2] > Reference_Lower_Value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		while (sensors[2] < Reference_Upper_Value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
		set_motors(0,0);
	}
		
			
	else if(x == 'S'){																																						//drive straight and adjusts the robot so it follows the line
		
		// The "proportional" term should be 0 when we are on the line.
		int proportional = ((int)position) - 2000;

		// Compute the derivative (change) and integral (sum) of the
		// position.
		int derivative = proportional - last_proportional;
		integral = integral + proportional;

		// Remember the last position.
		 last_proportional = proportional;
 
 		int power_difference = proportional/10 + integral/40000 + derivative*0.50;						

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
	
	
	else if(x == 'B'){																																							//Reverse while following the line
		set_motors(-50,-50);
		
		if (sensors[3] >=Reference_Upper_Value)
			set_motors(-50,-55);
		if (sensors[1] >=Reference_Upper_Value)
			set_motors(-55,-50);
	}
	
	
	else if(x == 'T'){																																							// Turn around
		set_motors(-turn_value,turn_value);
		
		while (sensors[2] <=Reference_Lower_Value)
			read_line_sensors_calibrated(sensors,IR_EMITTERS_ON);
	}
	
		
	else if(x == 'P')																																							//Robot stops slowly
		for( int i=50;i>=0;i-=3)
		{
			set_motors(i,i);
			delay(4);
		}
		
		
}



void manualControl(){																																							// function for manual control of the robot
static int speed = 0;
static unsigned int counter = 0;

updateDisplay(0,batteryPercentage(),manual);
	myComs->Update(myComs);
	if(myComs->Direction[0] == 1){											// increase speed
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed +=10;
		}
		if(speed > 100)
		{
			speed = 100;
		}
		counter++;
	}
	else if(myComs->Direction[0] == 0 && myComs->Direction[2] == 0)			// decrease speed without button press
	{
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed-=10;
		}
		if(speed < 1)
		{
			speed = 0;
		}
		counter++;
	}
	
	else if(myComs->Direction[2] == 1){										// Backwards(decrease speed)				
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed-=10;
		}
		if(speed < -100)
		{
			speed = -100;
		}
		counter++;
	}
	else if(myComs->Direction[0] == 1 && myComs->Direction[2] == 0)			// Increases button speed when below 0
	{
		set_motors(speed,speed);
		if(counter % 20 == 0)
		{
			speed += 10;
		}
		if(speed > -1)
		{
			speed = 0;
		}
		counter++;
	}
	
	if(myComs->Direction[1] == 1){											// turn left
		set_motors(speed/2,speed);
	}
	
	if(myComs->Direction[3] == 1){											// turn Right
		set_motors(speed,speed/2);
	}
}

void Spin(){																// spin fuction 
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