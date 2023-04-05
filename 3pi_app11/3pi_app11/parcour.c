 /*
 * parcour.c
 *
 * Created: 22/03/2023 11:02:27
 *  Author: joren
 */ 

#include "movementManagement.h"
#include "parcour.h"
#include "display.h"
#include "Comunication.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

Junctions situatie = Straight;

char route[20] = "_"; 
char routeBack[20];
int routea = 1;

int parcoursSearch(Communications *communications) // go to parcour with left hand rule
{
	char richting;
	while(1) 
	{
		situatie = lineType();
		
		switch (situatie) 
		{
			case Straight:
				motorControl('S');
				break;
					
			case Line_end:
				motorControl('T');
				richting = 'T';
				strncat(route, &richting, 1);
				break;
					
			case Barcode: // end of parcour
				motorControl('P');
				return 0;
			
			case Straight_right_junction:
				richting = 'S';
				strncat(route, &richting, 1);
				break;
			
			case X_junction:
			case Left_corner:
			case T_junction:
			case Straight_left_junction:
				motorControl('L');
				richting = 'L';
				strncat(route, &richting, 1);
				break;
			
			case Right_corner:
				motorControl('R');
				richting = 'R';
				strncat(route, &richting, 1);
				break;	
		}
		
		////// testing route
		/*clear();
		lcd_goto_xy(0,0);
		for (int i=0; i <= strlen(route);i++ ){
		print_character(route[i]);
		}*/
		
		////// find shortest route
		/*char richting;	
		if (route[strlen(route)-2] == 'T') 
		{
			char char_1 = route[strlen(route)-3];
			char char_2 = route[strlen(route)-1];

			if (char_1 == 'L' && char_2 == 'L') 
			{
				richting = 'S';
			}
			else if (char_1 == 'S' && char_2 == 'L')
			{
				richting = 'R';
			}
			else if (char_1 == 'R' && char_2 == 'L')
			{
				richting = 'T';
			}
			else if (char_1 == 'L' && char_2 == 'R')
			{
				richting = 'T';
			}
			else if (char_1 == 'L' && char_2 == 'S')
			{
				richting = 'R';
			}
			route[strlen(route)-3] = '\0';
			strncat(route, &richting, 1);
		}*/
	}
	routea = 0;	
	return 0;
}

////// run shortest route
/*int parcoursRun(char way, Communications *communications) 
{
	char richting;
	for (int i = 0; i < strlen(route); i++) 
	{
		while (lineType() == Straight)
		{
			motorControl('S');
		}
		if (way == 'T') {
			motorControl(route[i]);
		} 
		else if (way == 'B') 
		{
			richting = route[strlen(route)-i-1];
			switch (richting)
			{
				case 'R':
					richting = 'L';
					break;
				case 'L':
					richting = 'R';
					break;
			}
			motorControl(richting);
		}
	}
	return 0;
}*/

int parcours(char way, Communications *communications) 
{
	updateDisplay(0, batteryPercentage(), mazeSolver);
	if (route[0] == '_')// if it is the first time -> parcoursSearch
	{		
		memset(route,0,sizeof route);
		parcoursSearch(communications);
	} 
	else						// if it isn't the fist time -> run shortest route
	{
	//	parcoursRun(way, communications); // run shortest route
		parcoursSearch(communications);
	} 
	return 0;
}