/*
 * communications.h
 *
 * Created: 22/03/2023 11:11:04
 *  Author: bbelsenaar
 */ 


#ifndef COMUNICATION_H_
#define COMUNICATION_H_

typedef struct Communications {

//PRIVATE	

	void (*Close)(struct Communications *); 		// close handle
	int (*Send)(struct Communications *); 			// send msg
	int val;										// Lenght of array
	char msgBuffer[26];								// Buffer for msg to be send
	int SendSuccesfull;
	int (*Recieve)(struct Communications *);
	char Recieved[26];
	int newmsg;

//PUBLIC

	// LOCATIONS
	char Orderary[25];
	char Orderarx[25];
		
	// CHECK FOR UPDATES/MSGS
	char EmergancyStop; // OUTPUT  0 for clear 1 for stop
	char batterylvl;	// INPUT % of battery
	char magprocess;	// INPUT % of order completion
	char locationx;		// INPUT Location of robot x
	char locationy;		// INPUT Location of robot y
	void (*Update)(struct Communications *,int);
	
		


} Communications;



#endif /* COMUNICATION_H_ */