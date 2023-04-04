/*
 * comunication.c
 *
 * Created: 22/03/2023 11:10:47
 *  Author: bbelsenaar
 */ 
#include "Comunication.h"
#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const char welcomed[] PROGMEM = ">g32>>c32";

const char fugue[] PROGMEM =
"! O5 L16 agafaea dac+adaea fa<aa<bac#a dac#adaea f"
"O6 dcd<b-d<ad<g d<f+d<gd<ad<b- d<dd<ed<f+d<g d<f+d<gd<ad"
"L8 MS <b-d<b-d MLe-<ge-<g MSc<ac<a ML d<fd<f O5 MS b-gb-g"
"ML >c#e>c#e MS afaf ML gc#gc# MS fdfd ML e<b-e<b-"
"O6 L16ragafaea dac#adaea fa<aa<bac#a dac#adaea faeadaca"
"<b-acadg<b-g egdgcg<b-g <ag<b-gcf<af dfcf<b-f<af"
"<gf<af<b-e<ge c#e<b-e<ae<ge <fe<ge<ad<fd"
"O5 e>ee>ef>df>d b->c#b->c#a>df>d e>ee>ef>df>d"
"e>d>c#>db>d>c#b >c#agaegfe f O6 dc#dfdc#<b c#4";


#define START 010
#define STOP 101

void filllocationarrays(Communications *self){
	char arrx[25];
	char arry[25];
	char set;
	int placement=0;

	for (int i=0;i<25;i++){
	arrx[i]=arry[i]=self->Orderary[i]=self->Orderarx[i]=0;
}

	if (self->Recieved[1]==1){
		arrx[placement]=1;
		arry[placement]=1;
		placement++;
	}
	for (int i=0;i<8;i++){
		set = ((self->Recieved[2] >> i)  & 0x01);
		if (i<4){
			if (set == 1){
			arrx[placement]=i+1+1;
			arry[placement]=0+1;
			placement++;}
		}else{
			if (set == 1){
			arrx[placement]=i-4+1;
			arry[placement]=1+1;
			placement++;}
		}
	}
		for (int i=0;i<8;i++){
			set = ((self->Recieved[3] >> i)  & 0x01);
			if (i==0){
				if (set == 1){
				arrx[placement]=i+4+1;
				arry[placement]=1+1;
				placement++;}
			}else if(i<6){
				if (set == 1){
				arrx[placement]=i-1+1;
				arry[placement]=2+1;
				placement++;}
			}else{
				if (set == 1){
				arrx[placement]=i-6+1;
				arry[placement]=3+1;
				placement++;}
			}
		}
		for (int i=0;i<8;i++){
			set = ((self->Recieved[4] >> i)  & 0x01);
			if (i<3){
				if (set == 1){
				arrx[placement]=i+2+1;
				arry[placement]=3+1;
				placement++;}
			}else{
				if (set == 1){
				arrx[placement]=i-3+1;
				arry[placement]=4+1;
				placement++;}
			}
		}
	for (int i=0;i<25;i++){
		self->Orderarx[i]=arrx[i];
		self->Orderary[i]=arry[i];
	}
}
			


void sendACK(Communications *self ){
						char tmp[3];
						tmp[0]=START;
						tmp[1]=255;
						tmp[2]=STOP;
						serial_send_blocking(tmp,self->val);
}

void Update(Communications *self){
	char Flag =0;
			if (serial_receive_buffer_full()==1){
				if (self->Recieved[0]==8 && self->Recieved[self->val-1]==101){
				Flag = 1;
				//play_from_program_space(fugue);
				
				sendACK(self); // stops here to send acknowledge
				if (self->Recieved[5]==254 || self->EmergencyStop==1){	//enter emergency stop
									play_from_program_space(fugue);
					self->EmergencyStop=1;
				}
				
				if (self->Recieved[8]==250 && self->EmergencyStop==1){	// Leave emergency stop
					self->EmergencyStop=0;
				}
				
				if (self->Recieved[8]==245){							// Enter Spin Mode
					self->EmergencyStop=3;
				}
				if (self->Recieved[8]==240){							// Enter Manual Mode
					self->EmergencyStop=4;
					for (int i = 0; i < 4; i++){
						self->Direction[i]=self->Recieved[i+9];
					}
				}
				
				if (self->locationx==255 && self->locationy==255){
					filllocationarrays(self);
					self->flag =1;
				}
				}
			}
			
				for (char i=0;i<self->val;i++){
					self->msgBuffer[i]=i;
				}
				self->msgBuffer[0]=START;
				self->msgBuffer[1]=self->batterylvl;
				self->msgBuffer[2]=self->magprocess;
				self->msgBuffer[3]=self->locationx;
				self->msgBuffer[4]=self->locationy;
				self->msgBuffer[5]=self->EmergencyStop;
				self->msgBuffer[self->val-1]=STOP;
				serial_send(self->msgBuffer, self->val);

				//delay_ms(10000); // test
				if (Flag == 1){
					serial_receive(self->Recieved,self->val);
				}		
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

void Communications_INIT(Communications * myCom){
	myCom->val = Max_Bytes;	


	//Setup Communications
	serial_set_baud_rate(9600);
	serial_receive(myCom->Recieved,myCom->val);

	//filling in functions
	myCom->Update		 = Update;
		
	//filling in default values
	myCom->EmergencyStop = 0;
	myCom->batterylvl	 = 55;
	myCom->magprocess	 = 20;
	myCom->locationx	 = 4;
	myCom->locationy	 = 5;
	
	
	fillarrays(myCom);
}
