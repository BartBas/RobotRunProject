/*
 * warehouse.h
 *
 * Created: 12/03/2023 14:18:00
 *  Author: Joey
 */ 


#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

typedef struct
{
	int posX ;
	int posY ;
	char direction ;
}piRobot;

void warehouse(void);
void moveY(int orderPos);
void moveX(int orderPos);
void turnRobot(char direction, piRobot *myRobot);
void bubbleSort(int arr[], int orderY[], int n);
void swap(int *a, int *b);

#endif /* WAREHOUSE2_H_ */
