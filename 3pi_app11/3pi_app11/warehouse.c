/*
 * warehouse.c
 *
 * Created: 12/03/2023 14:18:00
 *  Author: Joey
 */ 
#include "warehouse.h"
//#include "motor.h"
//#include "sensor.h"
//#include <pololu/3pi.h>


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


void turnRobot(char direction, piRobot *myRobot)
{
    int changing = true;
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
                if(direction == 'W')
                {
                    motorControl('L');
                    myRobot->direction = 'W';
                }
                if(direction == 'S')
                {
                    motorControl('B');
                    myRobot->direction = 'S';
                }
                break;
            case 'E':
                if(direction == 'S')
                {
                    motorControl('R');
                    myRobot->direction = 'S';
                }
                if(direction == 'W')
                {
                    motorControl('L');
                    myRobot->direction = 'W';
                }
                if(direction == 'S')
                {
                    motorControl('B');
                    myRobot->direction = 'S';
                }
                break;
            case 'S':
                if(direction == 'W')
                {
                    motorControl('R');
                    myRobot->direction = 'W';
                }
                if(direction == 'E')
                {
                    motorControl('L');
                    myRobot->direction = 'E';
                }
                if(direction == 'N')
                {
                    motorControl('B');
                    myRobot->direction = 'N';
                }
                break;
            case 'W':
                if(direction == 'N')
                {
                    motorControl('R');
                    myRobot->direction = 'N';
                }
                if(direction == 'S')
                {
                    motorControl('L');
                    myRobot->direction = 'S';
                }
                if(direction == 'E')
                {
                    motorControl('B');
                    myRobot->direction = 'E';
                }
                break;
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
            turnRobot('W');
            junctions = orderPos-robot.posX;
        }
        else{
            turnRobot('E');
            junctions = robot.posX - orderPos;
        }

        for(int i = 0; i < junctions; i++)
        {
            motorControl('S');
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
            turnRobot('N');
            junctions = orderPos-robot.posY;
        }
        else{
            turnRobot('S');
            junctions = robot.posY - orderPos;
        }

        for(int i = 0; i < junctions; i++)
        {
            motorControl('S');
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
