/*
 *  Author: Walker Arce
 */ 

#include "capi324v221.h"

void CBOT_main(void)
{
	const unsigned char forward_data[] = { 0, 0, 0, 1 };
	const unsigned char reverse_data[] = { 0, 0, 1, 0 };
	
	uint8_t buttons;
	uint16_t left = 0;
	uint16_t right = 0;
	STEPPER_DIR left_direction = STEPPER_FWD;
	STEPPER_DIR right_direction = STEPPER_FWD;
	
    while(1)
    {
             LCD_open();
             PSXC_open();
			 
             PSXC_STDATA psxc_data;
			 SUBSYS_STATUS opuart;
			 
			 opuart = UART_open( UART_UART0 );

             while(1){
				 
				 STEPPER_move(	STEPPER_FREERUNNING, STEPPER_BOTH,
								left_direction, 1, left, 1000, STEPPER_BRK_OFF, NULL,
								right_direction, 1, right, 1000, STEPPER_BRK_OFF,NULL);
				 
				 DELAY_ms(5);
				 
	             // If data read is good...
	             if( PSXC_read( &psxc_data ) == TRUE ){
					 buttons = psxc_data.buttons0;
				
					 if( buttons == 239 ){
						left_direction = STEPPER_FWD;
						right_direction = STEPPER_FWD;
						left = 600;
						right = 600;
						 
						if( opuart == SUBSYS_OPEN ){
							unsigned int i;
							LCD_printf( "UART0 OPEN FORW\t");
							//Configure the UART 
							//8-bits, 1 stop bit, no parity, 9600 baud rate
							UART_configure( UART_UART0,
											UART_8DBITS, UART_1SBIT, UART_NO_PARITY, 9600 );
											
							//Enable the transmitter
							UART_set_TX_state( UART_UART0, UART_ENABLE );
							
							//Transmit forward string
							for ( i = 0; i < 4; i++)
								UART0_transmit( forward_data[i]);
						}
					 }
					if ( buttons == 191 ){
						left_direction = STEPPER_REV;
						right_direction = STEPPER_REV;
						left = 600;
						right = 600;
						
						if( opuart == SUBSYS_OPEN ){
							unsigned int i;
							LCD_printf( "UART0 OPEN BACK\t");
							//Configure the UART
							//8-bits, 1 stop bit, no parity, 9600 baud rate
							UART_configure( UART_UART0,
											UART_8DBITS, UART_1SBIT, UART_NO_PARITY, 9600 );
							
							//Enable the transmitter
							UART_set_TX_state( UART_UART0, UART_ENABLE );
							
							//Transmit forward string
							for ( i = 0; i < 4; i++){
								UART0_transmit( reverse_data[i]);
							}
						}
					}
				 }
				 
			 }
	}
}