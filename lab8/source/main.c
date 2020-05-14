/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggerin. Since we are 
	// 	  in Free Running mode, a new conversion will trigger whenever
	// 	  the previous conversion completes.
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // PORTA set to input.
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output.
	DDRD = 0xFF; PORTD = 0x00; // PORTD set to output.

	ADC_init();
	unsigned short my_adc = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempD = 0x00;

	while (1) {
		my_adc = ADC;

		tempB = (char)my_adc;

		tempD = (char)(my_adc >> 8);

		PORTB = tempB;
		PORTD = tempD;
	}
	return 1;
}
