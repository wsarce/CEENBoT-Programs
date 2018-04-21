/*	Accelerometer Test 
 *	Using a Sparkfun ADXL337 accelerometer breakout, the CEENBoT outputs the x, y, and z axis values using the ADC to process.
 *	Power:		J3 18 (3.3V)
 *	X-Axis:		J3 1 (ADC3)
 *	Y-Axis:		J3 2 (ADC4)
 *	Z-Axis:		J3 3 (ADC5)
 *	GND:		J3 20 (GND)
 *
 *	Designed for Prairie STEM.
 *  Author: Walker Arce
 *	Date: 9/17/2016
 */ 

/****PROTOTYPES****/
void xaxis(void);
void yaxis(void);
void zaxis(void);
/******************/

#include "capi324v221.h"

SUBSYS_STATUS ops_adc, ops_lcd;
ADC_SAMPLE xaxissample, yaxissample, zaxissample;
float xaxisv, yaxisv, zaxisv;

void CBOT_main(void)
{
    while(1)
    {
		//Initialize LCD for use
		ops_lcd = LCD_open();
		//Initialize ADC for use
		ops_adc = ADC_open();
		
		while(1){
			if((ops_adc == SUBSYS_OPEN) && ((ops_lcd == SUBSYS_OPEN) || (ops_lcd == SUBSYS_ALREADY_OPEN))){
				//Read x-axis
				xaxis();
				
				//Read y-axis
				yaxis();

				//Read z-axis
				zaxis();
			}
		}
    }
}

void xaxis(void){
	//Set the reference voltage to 3.3V
	ADC_set_VREF(ADC_VREF_AVCC);
	//Set to ADC channel 3, J3 pin 1
	ADC_set_channel(ADC_CHAN3);
	//Sample ADC channel
	xaxissample = ADC_sample();
	//Convert to voltage value
	//xaxisv = xaxissample * (3.3 / 1024);
	
	//Print found value
	LCD_printf_RC(3, 0, "X: %d \t", xaxissample);
	
	return;
}
void yaxis(void){
	//Set voltage reference to 3.3V 
	ADC_set_VREF(ADC_VREF_AVCC);
	//Set ADC channel 4, J3 pin 4
	ADC_set_channel(ADC_CHAN4);
	//Sample ADC channel
	yaxissample = ADC_sample();
	//Convert to voltage value
	//yaxisv = (yaxissample * (3.3 / 1024));
	
	//Print found value
	LCD_printf_RC(2, 0, "Y: %d \t", yaxissample);
	
	return;
}
void zaxis(void){
	//Set voltage reference to 3.3V
	ADC_set_VREF(ADC_VREF_AVCC);
	//Set ADC channel 5, J3 pin 3
	ADC_set_channel(ADC_CHAN5);
	//Sample ADC channel
	zaxissample = ADC_sample();
	//Convert to voltage value
	//zaxisv = zaxissample * (3.3/ 1024);
	
	//Print found value
	LCD_printf_RC(1, 0, "Z: %d \t", zaxissample);
	
	return;
}