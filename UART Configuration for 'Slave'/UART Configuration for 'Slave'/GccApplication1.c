/*
 *  Author: Walker Arce
 */ 

#include "capi324v221.h"

void CBOT_main(void)
{
	STEPPER_DIR left_direction = STEPPER_FWD;
	STEPPER_DIR right_direction = STEPPER_FWD;
	uint16_t left = 0;
	uint16_t right = 0;
	
	LCD_open();
	
	SUBSYS_STATUS opuart;
	opuart = UART_open( UART_UART0 );
	
    while(1)
    {
		if ( opuart == SUBSYS_OPEN ){
			unsigned int i;
			
			unsigned char temp;
			unsigned char buffer[ 4 ];
			
			LCD_printf( "UART0 OPEN");
			
			//Configure UART
			UART_configure( UART_UART0,
							UART_8DBITS, UART_1SBIT, UART_NO_PARITY, 9600 );
			
			//Set UART state
			UART_set_RX_state ( UART_UART0, UART_ENABLE );
			
			while (1){
				
				if ( UART_has_data( UART_UART0) == TRUE ){
				
					for ( i = 0; i < 4; i++ ){
						//Store single byte temporarily
						UART0_receive( &temp );
					
						//Store temp in buffer
						buffer[ i ] = temp;
					}
					
					if (buffer[ 2 ] == 0){
						if (buffer[ 3 ] == 1){
							left_direction = STEPPER_FWD;
							right_direction = STEPPER_FWD;
							left = 600;
							right = 600;
							
							STEPPER_move(	STEPPER_FREERUNNING, STEPPER_BOTH,
											left_direction, 1, left, 1000, STEPPER_BRK_OFF, NULL,
											right_direction, 1, right, 1000, STEPPER_BRK_OFF,NULL);
							DELAY_ms(5);
						}
					else if (buffer[ 2 ] == 1){
						if (buffer[ 3 ] == 0){
							left_direction = STEPPER_REV;
							right_direction = STEPPER_REV;
							left = 600;
							right = 600;
							
							STEPPER_move(	STEPPER_FREERUNNING, STEPPER_BOTH,
											left_direction, 1, left, 1000, STEPPER_BRK_OFF, NULL,
											right_direction, 1, right, 1000, STEPPER_BRK_OFF,NULL);
							DELAY_ms(5);
						}
					}
					}
				}
			}
		}
	}
}