/*	Author: Celvin Lizama Pena cliza007@ucr.edu
 *  Partner(s) Name: none
 *	Lab Section:22
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdlib.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
       				   // Initialize output on PORTD to 0x00

	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
	unsigned char tempD = 0x00;

	while(1) {
	tempA = PINA;
	tempB = PINB;
	tempC = PINC;

	unsigned char totalWeight = tempA + tempB +tempC;

	// Bits 7-2 hold the total weight. Bits 1 and 0 are left as 0.
	PORTD = totalWeight & 0xFC;
	
	// If the cart's total weight is greater than 140kg, set PD0 to 1.
	if(totalWeight > 140){
                tempD = 0x01;
        }

	// If the difference between A and C exceeds 80kg, set PD1 to 1.
	if(abs(tempA - tempC) > 80){
		tempD = tempD | 0x02;
	}
	
	// Put PD1 and PD0 into PORTD. PD7 - PD2 HOLD total weight.
	PORTD = PORTD | tempD;
	
	}
	return 1;
}
