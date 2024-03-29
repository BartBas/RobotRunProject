/*
 * display.h
 *
 * Created: 17/02/2023 17:35:42
 *  Author: Joey
 */ 
#ifndef DISPLAY_H_
#define DISPLAY_H_

typedef enum
{
	manual,//M
	mazeSolver,//L
	logicsBot,//O
	chargeMode,//C
	homingMode//H
}robotStates;

typedef enum
{
	slipped,
	emergency,
	object,
	batLow,
	}errorStates;


float batteryPercentage();
void updateDisplay(int orderPercent, int batteryPercent, robotStates currentState);
void errorDisplay(errorStates error, int batteryPercent);

#endif //DISPLAY_H_