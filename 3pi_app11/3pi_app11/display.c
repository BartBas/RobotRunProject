/*
 * display.c
 *
 * Created: 17/02/2023 17:32:56
 *  Author: Joey
 */ 

#include "display.h"
#include <pololu/3pi.h>

int batteryPercentage()//Should this function be in main or in display.c or something else?
{
	
	int milliVolts = read_battery_millivolts();
	int batteryPercent = milliVolts/50;
	return batteryPercent;
}

void updateDisplay(int orderPercent, int batteryPercent, robotStates currentState)
{
	clear();
	lcd_goto_xy(0,0);
	print("Task:");
	print_long(orderPercent);
	print("%");
	
	lcd_goto_xy(0,1);
	print("Bat:");
	print_long(batteryPercent);
	print("%");
	switch(currentState)
	{
		case manual:
		print("M");//M for Manual
		break;
		case mazeSolver:
		print("P");//P for Parcours
		break;
		case logicsBot:
		print("O");//O for Order
		break;
		case chargeMode:
		print("C");//C for Charge
		break;
		case homingMode:
		print("H");//H for Homing
		break;
	}
}

void errorDisplay(int error)
{
	
}