/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #8  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char MAX;
unsigned char THRESHOLD;

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggerin. Since we are 
	// 	  in Free Running mode, a new conversion will trigger whenever
	// 	  the previous conversion completes.
}

void lightLEDs(unsigned short temp_adc){
	if(temp_adc >= MAX ){
		PORTB = 0xFF;
	}	
	else if(temp_adc >= THRESHOLD * 7){
		PORTB = 0x7F;
	}
	else if(temp_adc >= THRESHOLD * 6){
		PORTB = 0x3F;
	}
	else if(temp_adc >= THRESHOLD * 5){
		PORTB = 0x1F;
	}
	else if(temp_adc >= THRESHOLD * 4){
		PORTB = 0x0F;
	}
	else if(temp_adc >= THRESHOLD * 3){
		PORTB = 0x07;
	}
	else if(temp_adc >= THRESHOLD * 2){
		PORTB = 0x03;
	}
	else if(temp_adc >= THRESHOLD * 1){
		PORTB = 0x01;
	}
	else{
		PORTB = 0x00;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	//DDRA = 0x00; PORTA = 0xFF; // PORTA set to input.
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output.
	DDRD = 0xFF; PORTD = 0x00; // PORTD set to output.
	
	MAX = 0x7F;
	THRESHOLD = MAX/8;
	
	ADC_init();
	unsigned short my_adc;

	while (1) {
		my_adc = ADC;

		lightLEDs(my_adc);

	}
	return 1;
}
