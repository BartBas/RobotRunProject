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



void drive(int junctions)
{
	Junctions situatie;
	motorControl('S');
	int driving = 1;
	int count = 0;
	while(driving) {
		situatie = lineType();
		//clear();
		switch (situatie) {
			case Barcode: // end of parcour
			//motorControl('N');
			break;
			
			case Straight:
			motorControl('S');
			break;
			
			
			case Line_end:
			case X_junction:
			case Left_corner:
			case T_junction:
			case Straight_left_junction:
			case Right_corner:
			case Straight_right_junction:
				count++;
				motorControl('S');
				if(count == junctions)
				{
					motorControl('P');
					driving = 0;
				}
				
				break;
			
			
			
		}
		
	}
}

void waitForTurn()
{
	Junctions line;
	delay(15);//Short delay so it won't accidentally think it found the line instantly
	while(line != Straight)
	{
		line  = lineType();
	}
	motorControl('N');
}

void turnRobot(char direction, piRobot *myRobot)
{
    int changing = 1;

    while(changing==1)
    {
		
        switch(myRobot->direction)
        {
            case 'N':
                if(direction == 'E')
                {
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'E';
                }
                else if(direction == 'W')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'W';
                }
                else if(direction == 'S')
                {
                    motorControl('T');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                break;
            case 'E':
                if(direction == 'S')
                {
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                else if(direction == 'W')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'W';
                }
                else if(direction == 'S')
                {
                    motorControl('T');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                break;
            case 'S':
                if(direction == 'W')
                {
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'W';
                }
                else if(direction == 'E')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'E';
                }
                else if(direction == 'N')
                {
                    motorControl('T');
					waitForTurn();
                    myRobot->direction = 'N';
                }
                break;
            case 'W':
                if(direction == 'N')
                {
					
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'N';
                }
                else if(direction == 'S')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                else if(direction == 'E')
                {
                    motorControl('T');
					waitForTurn();
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


void moveX(int orderPos, piRobot *robot)
{
	
    int junctions;
	
	
    if(robot->posX != orderPos)
    {
		
        if(robot->posX < orderPos)
        {
            turnRobot('W', robot);
            junctions = orderPos-robot->posX;
        }
        else{
            turnRobot('E', robot);
            junctions = robot->posX - orderPos;
        }

        
		drive(junctions);
        

    }
}

void moveY(int orderPos, piRobot *robot)
{

    int junctions;
    if(robot->posY != orderPos)
    {
        if(robot->posY < orderPos)
        {
            turnRobot('N', robot);
            junctions = orderPos-robot->posY;
        }
        else{
            turnRobot('S', robot);
            junctions = robot->posY - orderPos;
        }

            drive(junctions);
        

    }
}


void warehouse(objective objective)
{
 piRobot robot;
 robot.direction = 'W';
 robot.posX = -1;
 robot.posY = 0;
//int orderX[] = getWixel("posX");
//int orderY[] = getWixel("posY");

    int orderX[] = {0,3,1,2,4};
    int orderY[] = {1,2,3,3,4};

    int arraySize = sizeof(orderX) / sizeof(orderX[0]);
    bubbleSort(orderX, orderY, arraySize);
	
	
	 //motorControl('S');
	//drive();
	if(objective == orderPicking)
	{
		for(int locations = 0; locations<arraySize;locations++)
		{
			float completed = locations;
			float total = arraySize;
		
		
			moveX(orderX[locations], &robot);
			robot.posX = orderX[locations];
			moveY(orderY[locations], &robot);
			robot.posY = orderY[locations];
			delay(1000);//wait 1 second
		
			updateDisplay((((completed+1)/total)*100),batteryPercentage(),logicsBot);
		}
			moveY(0, &robot);
			moveX(0, &robot);
	}
	else
	{
		static int charged = 0;
		if(charged == 0)
		{
			drive(1);
			motorControl('R');
			drive(3);
			motorControl('R');
			drive(1);
			charged = 1;
		}
		else
		{
			motorControl('T');
			drive(1);
			motorControl('L');
			drive(3);
			motorControl('L');
			
		}
		
	}
    //goHome();
}
