/*
 *  Desc:	This program takes the Sparkfun Line Follower IC and has their inputs run into
 *			ADC channels 3, 4, and 5.  Their outputs are used to determine motor function.
 *  Author: Walker Arce
 */ 

#include "capi324v221.h"

void CBOT_main(void)
{
	LCD_open();
	
	SUBSYS_STATUS opstatadc, opstatstepper;
	
	opstatstepper = STEPPER_open();
	
	opstatadc = ADC_open();
	
	if (opstatadc == SUBSYS_OPEN)
	{
		if (opstatstepper == SUBSYS_OPEN)
		{
			while(1)
			{
				ADC_SAMPLE forwardsensor, firstsensor, thirdsensor;
			
				ADC_set_VREF(ADC_VREF_AVCC);
				//Set the ADC to the second IC
				ADC_set_channel(ADC_CHAN4);
			
				forwardsensor = ADC_sample();
				
				//If middle sensor is tripped, move forward
				if (forwardsensor > 900)
				{
					STEPPER_move(	STEPPER_STEP_BLOCK, STEPPER_BOTH,
									STEPPER_FWD, 100, 25, 25, STEPPER_BRK_OFF, NULL,
									STEPPER_FWD, 100, 25, 25, STEPPER_BRK_OFF, NULL);
					LCD_printf_RC(3, 1, "Forward: %d\t", forwardsensor);
				}

				//Set the ADC to the first IC
				ADC_set_channel(ADC_CHAN3);
			
				firstsensor = ADC_sample();
			
				//Set the ADC to the third IC
				ADC_set_channel(ADC_CHAN5);
			
				thirdsensor = ADC_sample();
				
				//If sensors trip, react accordingly
				//Turn left
				if (firstsensor > 900)
				{
					LCD_printf_RC(3, 1, "Left: %d\t", firstsensor);
					if (forwardsensor < 950)
					{
						STEPPER_move(	STEPPER_STEP_BLOCK, STEPPER_BOTH,
										STEPPER_REV, 100, 25, 25, STEPPER_BRK_OFF, NULL,
										STEPPER_FWD, 100, 25, 25, STEPPER_BRK_OFF, NULL);
					}
				}
				//Turn right
				if (thirdsensor > 900)
				{
					LCD_printf_RC(3, 1, "Right: %d\t", thirdsensor);
					if (forwardsensor < 950)
					{
						STEPPER_move(	STEPPER_STEP_BLOCK, STEPPER_BOTH,
										STEPPER_FWD, 100, 25, 25, STEPPER_BRK_OFF, NULL,
										STEPPER_REV, 100, 25, 25, STEPPER_BRK_OFF, NULL);
					}	
				}
			}
		}
	}
}