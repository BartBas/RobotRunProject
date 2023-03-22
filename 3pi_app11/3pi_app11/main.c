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


int main()
{
	clear();
	pololu_3pi_init(2000);
	initialize();
	
	
	
	/*char parecour[20];
	parcour(parecour);
	print(parecour);*/
	warehouse();
}