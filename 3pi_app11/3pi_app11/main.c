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

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const char welcome[] PROGMEM = ">g32>>c32";

int main()
{
	pololu_3pi_init(2000);
	play_from_program_space(welcome);
	initialize();
	
	while(1) 
	{
		updateDisplay(0, batteryPercentage(), homingMode);
		if (read_battery_millivolts_3pi() < 0.6 * 5200) 
		{
			charge_3pi();
		} else { //  if order
			pickOrder();
		}
	}
	
}

int charge_3pi() 
{
	whileBarcode();
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours('t');
	updateDisplay(0, batteryPercentage(), chargeMode);
	warehouse(charging);
	// while robot full
	warehouse(charging);
	whileBarcode();
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours('b');
	whileLineEnd();
	return 0;
}

int pickOrder() 
{
	motorControl('S');
	whileBarcode();
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours('t');
	updateDisplay(0, batteryPercentage(), orderPicking);
	warehouse(orderPicking);
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours('b');
	motorControl('S');
	whileLineEnd();
	motorControl('P');
	return 0;
}

int whileBarcode() 
{
	while (lineType() != Barcode)
	{
		motorControl('S');
	}
	return 0;
}

int whileLineEnd() 
{
	while (lineType() != Line_end)
	{
		motorControl('S');
	}
	return 0;
}