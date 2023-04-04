/*
 * parcour.c
 *
 * Created: 12-3-2023 12:05:39
 *  Author: Joost
 */ 

#include "warehouse.h"
#include "display.h"
#include "movementManagement.h"
#include "parcour.h"
#include "comunication.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const char welcome[] PROGMEM = ">g32>>c32";

void whileBarcode()
{
	while (lineType() != Barcode)
	{
		motorControl('S');
	}
}

void whileLineEnd()
{
	while (lineType() != Line_end)
	{
		motorControl('S');
	}
}
void charge_3pi(Communications *communications)
{
	whileBarcode();
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours('T', communications);
	updateDisplay(0, batteryPercentage(), chargeMode);
	warehouse(charging, communications);
	delay(5000);								// wacht totdat de robot vol is
	warehouse(charging, communications);
	whileBarcode();
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours('B', communications);
	whileLineEnd();
	motorControl('L');
}

void pickOrder(Communications *communications)
{
	whileBarcode();
	parcours('T', communications);
	warehouse(orderPicking, communications); 	
	parcours('B', communications);
	whileLineEnd();
	motorControl('L');
}

int main()
{
	play_from_program_space(welcome);
	
	Communications communications;
	Communications_INIT(&communications);
	
	initialize(&communications);
	
		
	while(1) 
	{
		//updateDisplay(0, batteryPercentage(), homingMode);
		communications.Update(&communications);
		
		communications.locationx = -1;
		communications.locationy = -1;
		
		lcd_goto_xy(0, 0);
		print_long(communications.EmergencyStop);
		
		switch (communications.EmergencyStop) {
			
			case 0: //0 for clear; 1 for stop; 2 for spin; 3 for manual
				/*if (read_battery_millivolts_3pi() < 0.6 * 5200)
				{
					charge_3pi(&communications);
				}
				else*/
				print_long(communications.locationx);
				print_long(communications.locationy);
				if (communications.flag==1)
				{
					
					play_from_program_space(welcome);
					communications.locationx = 0;
					communications.locationy = 0;
					pickOrder(&communications);
					communications.locationx = -1;
					communications.locationy = -1;
				}
				break;
				delay(100);
			case 2:
				Spin();
				break;
			case 3:
				manualControl();
				break;
				
		}
	}
	
}



