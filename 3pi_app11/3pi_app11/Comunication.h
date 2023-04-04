/*
 * communications.h
 *
 * Created: 22/03/2023 11:11:04
 *  Author: bbelsenaar
 */ 


#ifndef COMUNICATION_H_
#define COMUNICATION_H_

#define Max_Bytes 26 // BYTES send and received per message

typedef struct Communications {

//PRIVATE	

	char val;												// Length of message buffer
	char msgBuffer[Max_Bytes];								// Buffer for message to be send
	char Recieved[Max_Bytes];								// Buffer for message to be received

//PUBLIC
	
	//	MANUAL controls
	char Direction[4];	// OUTPUT	0 for button not pressed; 1 for button pressed. {UP, LEFT, DOWN, RIGHT}
	
	// LOCATIONS
	char Orderary[25];	// OUTPUT	array of order locations for robot x
	char Orderarx[25];	// OUTPUT	array of order locations for robot x
	
	// CHECK FOR UPDATES/MSGS
	char EmergencyStop; // OUTPUT	0 for clear; 1 for stop; 2 for spin; 3 for manual
	char batterylvl;	// INPUT	% of battery
	char magprocess;	// INPUT	% of order completion
	char locationx;		// INPUT	Location of robot x;  set to -1 for home -2 for order completion
	char locationy;		// INPUT	Location of robot y;  set to -1 for home -2 for order completion
	char flag;			// OUTPUT	

	void (*Update)(struct Communications *);
	
} Communications;

void Communications_INIT(struct Communications*);


#endif /* COMUNICATION_H_ */