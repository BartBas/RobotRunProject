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
	manualControl,//M
	mazeSolver,//L
	logicsBot,//O
	chargeMode,//C
	homingMode//H
}robotStates;


int batteryPercentage();
void updateDisplay(int orderPercent, int batteryPercent, robotStates currentState);
void errorDisplay();

#endif //DISPLAY_H_