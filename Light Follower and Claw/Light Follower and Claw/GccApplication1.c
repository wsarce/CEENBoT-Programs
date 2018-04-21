/*
 *  Author: Walker Arce
 */ 

#include "capi324v221.h"

//---------------------
void CBOT_main( void );
ADC_SAMPLE check_ADCl( void );
ADC_SAMPLE check_ADCr( void );
unsigned int motor_move( ADC_SAMPLE left, ADC_SAMPLE right );
//---------------------

void CBOT_main( void )
{
	//Set the data direction of I/O PORTA
	DDRA = 0;
	
	//Structure to store steps
	unsigned int recleft, recright;
	
	//Set the open status for the ADC
	SUBSYS_STATUS ops_adc, ops_stepper, op_attiny;
	
	//Open ATTINY module for RC Servo
	op_attiny = ATTINY_open();
					
	//Initialize the ADC module
	ops_adc = ADC_open();
	
	//Open the LCD.
	LCD_open();
	
	//Set the open status for the stepper motors
	ops_stepper = STEPPER_open();
	
	while(1)
	{	//If ADC is open fall into if statement
		if ( ops_adc == SUBSYS_OPEN )
		{
			//Initialize the ADC_SAMPLEs
			ADC_SAMPLE left;
			ADC_SAMPLE right;
			
			//Set the sample values with functions
			left = check_ADCl();
			right = check_ADCr();
			
			//Subtract observed physical minimums of circuit
			left -= 72;
			right -= 96;
			
			//If left value is less than zero, set it to zero
			if ( left < 0 )
			{
				left = 0;
			}
			//Else subtract is by found adjustment value
			else
			{
				left /= 2.353;
			}
			//If right value is less than zero, set it to zero
			if ( right < 0 )
			{
				right = 0;
			}
			//Else divide it by adjustment value
			else
			{
				right /= 2.353;
			}
			//Print values to LCD screen
			LCD_printf_RC(3, 6, "LEFT  RIGHT\t");
			LCD_printf_RC(2, 0, "SPEED: %d   %d\t", left, right);
			
			//Check if stepper module is open
			if ( ops_stepper == SUBSYS_OPEN )
			{
				//Call function with left and right sample variables
				motor_move( left, right );
			}
			
			if ((right > 950) && (left > 950))
			{
				if(op_attiny != SUBSYS_OPEN )
				{	
					// Set servo to close claw
					ATTINY_set_RC_servo(RC_SERVO0, 700);
				}
			}
			
			//Turn around
			STEPPER_move (	STEPPER_STEP_BLOCK, STEPPER_BOTH,
							STEPPER_REV, 200, 200, 200, STEPPER_BRK_OFF, NULL,
							STEPPER_FWD, 200, 200, 200, STEPPER_BRK_OFF, NULL);
							
			//Go back
			STEPPER_move (	STEPPER_STEP_BLOCK, STEPPER_BOTH,
							STEPPER_FWD, recleft, 200, 200, STEPPER_BRK_OFF, NULL,
							STEPPER_FWD, recright, 200, 200, STEPPER_BRK_OFF, NULL);
		} //End if()
	} //End while(1)
} //End CBOT_main()

ADC_SAMPLE check_ADCl( void )
{
	//Create sample variable
	ADC_SAMPLE saml;
	
	//Set the voltage reference first so VREF=5V
	ADC_set_VREF(ADC_VREF_AVCC);
	
	//Set the channel that will be sampled
	ADC_set_channel(ADC_CHAN3);
	
	//Sample it
	saml = ADC_sample();
	
	//Return the ADC sample
	return saml;
} //End check_ADCl()

ADC_SAMPLE check_ADCr( void )
{
	//Create sample variable
	ADC_SAMPLE samr;
	
	//Set the voltage reference first so VREF=5V
	ADC_set_VREF(ADC_VREF_AVCC);
	
	//Set the channel that will be sampled
	ADC_set_channel(ADC_CHAN4);
	
	//Sample it
	samr = ADC_sample();
	
	//Return the ADC sample
	return samr;
} //End check_ADCr()

unsigned int motor_move( ADC_SAMPLE left, ADC_SAMPLE right )
{
	unsigned int recleft, recright;
	
	//Set the operating 'run-mode'
	STEPPER_set_mode( STEPPER_BOTH, STEPPER_FREERUNNING);
	
	//Set the direction for both wheels
	STEPPER_set_dir2( STEPPER_FWD, STEPPER_FWD );
	
	//Set the acceleration of the motors
	STEPPER_set_accel2( 300, 300 );
	
	//Set the velocity with the found values
	STEPPER_set_speed2( left, right );
	
	recleft += left;
	recright += right;
	
	//Return values
	return recleft, recright;
} //End motor_move()