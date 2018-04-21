/*
 *  Author: Walker Arce
 *	This simple code uses an int variable to bit-bang PWM through PA3-7.
 */ 

int controller_input(void);

#include "capi324v221.h"

PSXC_STDATA psxc_data;
int pwm = 0x00;

void CBOT_main(void)
{
	SUBSYS_STATUS opstat;
	opstat = PSXC_open();
	
	DDRA = 0xF8;
	if (opstat == SUBSYS_OPEN){
		while(1){
			pwm -= 1;
			if(pwm == 0){
				PORTA ^= (1<<PA3);
			}
			while(PSXC_read ( &psxc_data) == TRUE){
				controller_input();
			}
		}
	}
}

int controller_input(){
	if( !(psxc_data.buttons1 & X_BIT)){
		pwm = 0x25;
	}
	if( !(psxc_data.buttons1 & CIR_BIT)){
		pwm = 0x50;
	}
	if( !( psxc_data.buttons1 & SQR_BIT)){
		pwm = 0x75;
	}
	if( !( psxc_data.buttons1 & TRI_BIT)){
		pwm = 0x100;
	}
	return pwm;
}
			
		
				