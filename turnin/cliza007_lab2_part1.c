/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: none
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #1
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
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
       				   // Initialize output on PORTB to 0x00

	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;

	while(1) {
	tempA = PINA & 0x03;

	if(tempA==0x01){
		tempB = 0x01;
	}
	else{
		tempB = 0x00;
	}

	PORTB = tempB;	// Writes port B's 8 pins with the values
				// on port A's 8 pins

	}
	return 1;
}
