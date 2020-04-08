/*	Author: cliza007
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
	unsigned char tempC = 0x00;

	while(1) {
	tempA = PINA & 0x0F;
	unsigned char i;
	unsigned char cntavail = 0x00;
	
	if(tempA == 0x0f){
                tempC = 0x80;
        }

	for(i = 0; i < 4; i++){
		if((tempA & 0x01) == 0x00){
			cntavail++;
		}
		tempA /= 2;
	}
	

	PORTC = tempC | cntavail;	

	}
	return 1;
}
