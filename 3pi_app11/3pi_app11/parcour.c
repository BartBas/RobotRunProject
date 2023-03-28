/*
 * parcour.c
 *
 * Created: 22/03/2023 11:02:27
 *  Author: joren
 */ 

#include "movementManagement.h"
#include "parcour.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

Junctions situatie = Straight;

char route[20] = "RSLR"; // "_"
char routeBack[20];

int parcoursSearch() 
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
			
			case Straight_right_junction:
				richting = 'S';
				strncat(route, &richting, 1);
				break;
				
			case Line_end:
				motorControl('T');
				richting = 'T';
				strncat(route, &richting, 1);
				break;
				
			case Barcode: // end of parcour
				motorControl('P');
				return 0;
		}
		
		
		char richting;
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
		}
	}
	return 0;
}

int parcoursRun(char way) 
{
	char richting;
	for (int i = 0; i < strlen(route); i++) 
	{
		while (lineType() == Straight)
		{
			motorControl('S');
		}
		if (way == 't') {
			motorControl(route[i]);
		} 
		else if (way == 'b') 
		{
			richting = route[strlen(route)-i];
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
}

int parcours(char way) 
{
	if (route == "_") 
	{
		parcoursSearch();
	} 
	else 
	{
		parcoursRun(way);
	} 
	return 0;
}