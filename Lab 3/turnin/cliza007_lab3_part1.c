/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: none
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #1
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
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;

	/* Insert your solution below */
	while (1) {
		unsigned char count = 0x00;
		unsigned char i;

		tempA = PINA;
		tempB = PINB;

		for(i = 0; i < 8; i++){
			if(tempA & 0x01 == 1){
				count++;
			}
			if(tempB & 0x01 == 1){
				count++;
			}
			tempA = tempA >> 1;
			tempB  = tempB >> 1;
		}
		PORTC = count;
					
	}
	return 1;
}
