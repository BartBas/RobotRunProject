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
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcoursSearch();
	
	while(1) 
	{
		if (read_battery_millivolts_3pi() < 0.6 * 5200) {
			charge_3pi();
		} else {
			// if order
			motorControl('S');
			while (lineType() != Barcode){}
			parcours();
			warehouse();
			parcours();
			motorControl('S');
			while (lineType() != Line_end){}
		}
	}
	
}

void charge_3pi() {
	motorControl('S');
	while (lineType() == Barcode){}
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours();
	motorControl('R');
	motorControl('S');
	motorControl('S');
	motorControl('S');
	motorControl('R');
	while (lineType() != Line_end){}
	updateDisplay(0, batteryPercentage(), chargeMode);
	// while robot full
	motorControl('L');
	motorControl('L');
	motorControl('S');
	motorControl('S');
	motorControl('S');
	motorControl('L');
	while (lineType() != Barcode){}
	updateDisplay(0, batteryPercentage(), mazeSolver);
	parcours();
	motorControl('S');
}