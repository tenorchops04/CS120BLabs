/*	Author: Celvin Lizama Pena cliza007@ucr.edu
 *  Partner(s) Name: none
 *	Lab Section: 22
 *	Assignment: Lab #2  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
       				   // Initialize output on PORTC to 0x00

	unsigned char tempA = 0x00;

	while(1) {
	tempA = PINA & 0x0F;
	unsigned char i;
	unsigned char cntavail = 0x00;

	for(i = 0; i < 4; i++){
		if((tempA & 0x01) == 0x00){
			cntavail++;
		}
		tempA /= 2;
	}
	
	PORTC = cntavail;	// Writes port C's 8 pins with the values
				// on port C's 8 pins

	}
	return 1;
}
