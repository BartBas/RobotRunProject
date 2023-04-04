/*
 * warehouse.h
 *
 * Created: 12/03/2023 14:18:00
 *  Author: Joey
 */ 


#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_
#include "Comunication.h"
typedef struct
{
	int posX;
	int posY;
	char direction;
}piRobot;

typedef enum
{
	orderPicking,
	charging
}objective;

void warehouse(objective objective, Communications *myCom);
void moveY(int orderPos, piRobot *robot, Communications *myCom);
void moveX(int orderPos, piRobot *robot, Communications *myCom);
void turnRobot(char direction, piRobot *myRobot);
void bubbleSort(char arr[], char orderY[], int n);
void swap(char *a, char *b);

#endif /* WAREHOUSE2_H_ */
