/*
 * comunication.c
 *
 * Created: 22/03/2023 11:10:47
 *  Author: bbelsenaar
 */ 
#include "Comunication.h"

int Send(Communications *self){
	return 1;
}
int Recieve(Communications *self){
	return 1;
}

void Update(Communications *self){
	
}

void fillarrays(Communications *self){
	//temp
		char arrx[]={2,5,4,2,1,6,1,5};
		char arry[]={0,1,3,5,2,4,1,2};
		for (int i=0;i<8;i++){
			self->Orderarx[i]=arrx[i];
			self->Orderary[i]=arry[i];
		}
}

Communications COMS_INIT(){
Communications myCom;
	myCom.Send=Send;
	myCom.Recieve=Recieve;
	fillarrays(&myCom);
	myCom.Update=Update;
return myCom;	
}
