/*
 * parcour.c
 *
 * Created: 12-3-2023 12:05:39
 *  Author: Joost
 */ 


unsigned int sensors[5]; // an array to hold sensor values
unsigned int position;

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

Junctions junction;


#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const char levels[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

void load_custom_characters()
{
	lcd_load_custom_character(levels+0,0); // no offset, e.g. one bar
	lcd_load_custom_character(levels+1,1); // two bars
	lcd_load_custom_character(levels+2,2); // etc...
	lcd_load_custom_character(levels+3,3);
	lcd_load_custom_character(levels+4,4);
	lcd_load_custom_character(levels+5,5);
	lcd_load_custom_character(levels+6,6);
	clear(); // the LCD must be cleared for the characters to take effect
}

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
	unsigned int sensors[5]; // an array to hold sensor values
	
	pololu_3pi_init(2000);
	load_custom_characters(); // load the custom characters
	
	while(!button_is_pressed(BUTTON_B))
	{
		int bat = read_battery_millivolts();
		
		clear();
		print_long(bat);
		print("mV");
		lcd_goto_xy(0,1);
		print("Press B");
		
		delay_ms(100);
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
	
	// Display calibrated values as a bar graph.
	while(!button_is_pressed(BUTTON_B))
	{
		// Read the sensor values and get the position measurement.
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);
		
		// Display the position measurement, which will go from 0
		// (when the leftmost sensor is over the line) to 4000 (when
		// the rightmost sensor is over the line) on the 3pi, along
		// with a bar graph of the sensor readings.  This allows you
		// to make sure the robot is ready to go.
		clear();
		print_long(position);
		lcd_goto_xy(0,1);
		display_readings(sensors);
		
		delay_ms(100);
	}
	wait_for_button_release(BUTTON_B);
	
	clear();
	
	
}

void inch(){
	set_motors(50,50);
	delay(150);
	set_motors(0,0);
}

char direction(){																												// function that returns the type of junction it detects
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
					motorcontrol('S');
					}		// checks if it detects the "barcode"
				return Barcode;
			}
			else 
				return Straight;																						// check if its straight without any corners
						
		}
		
	}
}


void motorcontrol(char x){
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
		
		if (sensors[3] >=400  && sensors[4] <=400 )
			set_motors(100,40);
		if (sensors[1] >=400 && sensors[0] <=400  )
			set_motors(40,100);
	}
	
	else if(x == 'B'){									// reverse
		set_motors(-50,-50);
		if (sensors[3] >=400)
			set_motors(-50,-55);
		if (sensors[1] >=400)
			set_motors(-55,-50);
	}
	else if(x == 'T'){
		set_motors(-75,75);
		while (sensors[2] >=500){
			read_line(sensors,IR_EMITTERS_ON);
		}
	}
}
 


char route[20];

Junctions situatie = Straight;

void parcour() {
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
	}
}
		


int main()
{
	pololu_3pi_init(2000);
	initialize();
	parcour();
}