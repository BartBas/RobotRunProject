/*
 * display.c
 *
 * Created: 17/02/2023 17:32:56
 *  Author: Joey
 */ 

#include "display.h"
#include <pololu/3pi.h>

float batteryPercentage()//Should this function be in main or in display.c or something else?
{
	
	float milliVolts = read_battery_millivolts();
	float batteryPercent = 0.000000000632*((milliVolts)*(milliVolts)*(milliVolts));
	return batteryPercent;
}

void updateDisplay(int orderPercent, int batteryPercent, robotStates currentState)
{
	clear();
	lcd_goto_xy(0,0);
	print("Job:");
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
		print("P");//P for Parcour
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

void errorDisplay(errorStates error, int batteryPercent)
{
	clear();
	lcd_goto_xy(0,0);
	print("Bat:");
	print_long(batteryPercent);
	print("%");
	
	lcd_goto_xy(0,1);
	
	switch(error)
	{
		case slipped:
		print("Slipped!");
		break;
		case emergency:
		print("STOPPED");
		break;
		case object:
		print("OBJECT");
		break;
		case batLow:
		print("LOW-BAT");
		break;
		case NoConnection:
		print("Con-Fail");
		break;
	}
}