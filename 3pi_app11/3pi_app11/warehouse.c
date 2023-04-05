/*
 * warehouse.c
 *
 * Created: 12/03/2023 14:18:00
 *  Author: Joey
 */ 
#include "warehouse.h"
#include "display.h"
#include "movementManagement.h"
#include "comunication.h"
//#include "motor.h"
//#include "sensor.h"

#include <pololu/3pi.h>
#include <avr/pgmspace.h>


void swap(char *a, char *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(char arr[],char orderY[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swap(&orderY[j], &orderY[j + 1]);
				
            }
		}
    }
}



void drive(int junctions, piRobot *robot, Communications *myCom)
{
	Junctions situatie;
	
	motorControl('S');
	int driving = 1;
	int count = 0;//Count for number of junctions to pass
	while(driving) {
		situatie = lineType();
		switch (situatie) {
			case Barcode: // end of parcour
			
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
				myCom->Update(myCom);
				break;	
		}
	}
}

void waitForTurn()
{
	Junctions line;
	line = lineType();
	delay(15);//Short delay so it won't accidentally think it found the line instantly
	while(line != Straight)
	{
		line  = lineType();
	}
	motorControl('N');
}

void turnRobot(char tdirection, piRobot *myRobot)
{
    int changing = 1;

    while(changing==1)
    {
		
        switch(myRobot->direction)
        {
            case 'N':
                if(tdirection == 'E')
                {
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'E';
                }
                else if(tdirection == 'W')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'W';
                }
                else if(tdirection == 'S')
                {
                    motorControl('L');motorControl('L');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                break;
            case 'E':
                if(tdirection == 'S')
                {
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                else if(tdirection == 'W')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'W';
                }
                else if(tdirection == 'S')
                {
                    motorControl('L');motorControl('L');
					waitForTurn();
                    myRobot->direction = 'S';
                }
                break;
            case 'S':
                if(tdirection == 'W')
                {
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'W';
                }
                else if(tdirection == 'E')
                {
                    motorControl('L');
					waitForTurn();
                    myRobot->direction = 'E';
                }
                else if(tdirection == 'N')
                {
                    motorControl('L');motorControl('L');
					waitForTurn();
                    myRobot->direction = 'N';
                }
                break;
            case 'W':
                if(tdirection == 'N')
                {
					
                    motorControl('R');
					waitForTurn();
                    myRobot->direction = 'N';
                }
                else if(tdirection == 'S')
                {
	                motorControl('L');
	                waitForTurn();
	                myRobot->direction = 'S';
                }
                else if(tdirection == 'E')
                {
                    motorControl('L');
					motorControl('L');
					waitForTurn();
                    myRobot->direction = 'E';
                }
                break;
		}
				if(myRobot->direction == tdirection)
				{
					changing = 0;
				}
		
    }
}


void moveX(int orderPos, piRobot *robot, Communications *myCom)
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
		drive(junctions, robot, myCom);
    }
}

void moveY(int orderPos, piRobot *robot, Communications *myCom)
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
            drive(junctions, robot, myCom);
        
    }
}


void warehouse(objective objective, Communications *myCom)
{
 piRobot robot;
 robot.direction = 'W';
 robot.posX = 0;
 robot.posY = 1;
 myCom->locationx = 0;
 myCom-> locationy = 1;
 
 //char orderX[]={2,5,4,3,1};
 //char orderY[]={2,1,3,5,2};
 //int arraySize = 5;
  char orderX[25];
  char orderY[25];
 int arraySize = 0;
 for(int i = 0; i<25;i++)
 {
 	if(myCom->Orderarx[i] != 0)
 	{
 		arraySize++;
 	}
 }
 for(int i=0;i<arraySize;i++)//Getting order positions from the wixel
 {
 	orderX[i] = myCom->Orderarx[i];
 	orderY[i] = myCom->Orderary[i];
 }


    bubbleSort(orderX, orderY, arraySize);//Sorts the order positions so that the lowest X position comes first
	
	if(objective == orderPicking)//If statement for when the robot just needs to quickly pass the warehouse to get to the charging station
	{
		for(int locations = 0; locations<arraySize;locations++)
		{
			float completed = locations;
			float total = arraySize;
			
			moveX(orderX[locations], &robot, myCom);
			robot.posX = orderX[locations];
			myCom->locationx = robot.posX;
			moveY(orderY[locations], &robot, myCom);
			robot.posY = orderY[locations];
			myCom->locationy = robot.posY;
			delay(1000);//wait 1 second
			
			myCom->batterylvl = batteryPercentage();
			myCom->magprocess = (((completed+1)/total)*100);
			myCom->Update(myCom);
			updateDisplay((((completed+1)/total)*100),batteryPercentage(),logicsBot);
		}
		moveY(1, &robot, myCom);
			moveX(1, &robot, myCom);
			
			
	}
	else
	{
		static int charged = 0;
		if(charged == 0)
		{
			drive(1, &robot, myCom);
			motorControl('R');
			drive(3, &robot, myCom);
			motorControl('R');
			drive(1, &robot, myCom);
			charged = 1;
		}
		else
		{
			motorControl('T');
			drive(1, &robot, myCom);
			motorControl('L');
			drive(3, &robot, myCom);
			motorControl('L');
			
		}
		
	}
}
