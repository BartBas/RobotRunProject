/*
 * warehouse.c
 *
 * Created: 12/03/2023 14:18:00
 *  Author: Joey
 */ 
#include "warehouse.h"
#include "display.h"
#include "movementManagement.h"
//#include "motor.h"
//#include "sensor.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int arr[],int orderY[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swap(&orderY[j], &orderY[j + 1]);
				
            }
		}
    }
}



void drive()
{
	Junctions situatie;
	while(1) {
		situatie = direction();
		
		switch (situatie) {
			case Barcode: // end of parcour
			motorControl('N');
			break;
			
			case Straight:
			motorControl('S');
			break;
			
			case X_junction:
			case Left_corner:
			case T_junction:
			case Straight_left_junction:
			case Right_corner:
			case Straight_right_junction:
			motorControl('N');
			
			break;
			
			
			case Line_end:
			motorControl('T');
			break;
		}
		
	}
}

void turnRobot(char direction, piRobot *myRobot)
{
    int changing = 1;
    while(changing)
    {
        switch(myRobot->direction)
        {
            case 'N':
                if(direction == 'E')
                {
                    motorControl('R');
                    myRobot->direction = 'E';
                }
                else if(direction == 'W')
                {
                    motorControl('L');
                    myRobot->direction = 'W';
                }
                else if(direction == 'S')
                {
                    motorControl('T');
                    myRobot->direction = 'S';
                }
                break;
            case 'E':
                if(direction == 'S')
                {
                    motorControl('R');
                    myRobot->direction = 'S';
                }
                else if(direction == 'W')
                {
                    motorControl('L');
                    myRobot->direction = 'W';
                }
                else if(direction == 'S')
                {
                    motorControl('T');
                    myRobot->direction = 'S';
                }
                break;
            case 'S':
                if(direction == 'W')
                {
                    motorControl('R');
                    myRobot->direction = 'W';
                }
                else if(direction == 'E')
                {
                    motorControl('L');
                    myRobot->direction = 'E';
                }
                else if(direction == 'N')
                {
                    motorControl('T');
                    myRobot->direction = 'N';
                }
                break;
            case 'W':
                if(direction == 'N')
                {
                    motorControl('R');
                    myRobot->direction = 'N';
                }
                else if(direction == 'S')
                {
                    motorControl('L');
                    myRobot->direction = 'S';
                }
                else if(direction == 'E')
                {
                    motorControl('T');
                    myRobot->direction = 'E';
                }
                break;
		}
		
				if(myRobot->direction == direction)
				{
					changing = 0;
				}
		
    }
}


void moveX(int orderPos)
{
	piRobot robot;
    int junctions;
    if(robot.posX != orderPos)
    {
        if(robot.posX < orderPos)
        {
            turnRobot('W', &robot);
            junctions = orderPos-robot.posX;
        }
        else{
            turnRobot('E', &robot);
            junctions = robot.posX - orderPos;
        }

        for(int i = 0; i < junctions; i++)
        {
			drive();
        }

    }
}

void moveY(int orderPos)
{
	piRobot robot;
    int junctions;
    if(robot.posY != orderPos)
    {
        if(robot.posY < orderPos)
        {
            turnRobot('N', &robot);
            junctions = orderPos-robot.posY;
        }
        else{
            turnRobot('S', &robot);
            junctions = robot.posY - orderPos;
        }

        for(int i = 0; i < junctions; i++)
        {
            drive();
        }

    }
}


void warehouse(void)
{
 piRobot robot;
 robot.direction = 'W';
 robot.posX = 0;
 robot.posY = 0;
//int orderX[] = getWixel("posX");
//int orderY[] = getWixel("posY");

    int orderX[] = {8,5,2,9,7,1,3};
    int orderY[] = {8,6,3,5,2,0,8};

    int arrayGrootte = sizeof(orderX) / sizeof(orderX[0]);
    bubbleSort(orderX, orderY, arrayGrootte);
	
	
	 motorControl('S');

    for(int locaties = 0; locaties<arrayGrootte;locaties++)
    {
        moveX(orderX[locaties]);
        moveY(orderY[locaties]);
        delay(1000);//wait 1 second
        updateDisplay(((locaties/arrayGrootte)*100),batteryPercentage(),logicsBot);
    }
    moveY(0);
    moveX(0);
    //goHome();
}
