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
	DDRB = 0x06; PORTB = 0x00;
	DDRD = 0x00; PORTD = 0x00;
		
	unsigned char tempB = 0x00;
	unsigned char tempD = 0x00;

	/* Insert your solution below */
	while (1) {
		tempD = PIND;
		tempB = PINB & 0x01;
		
		/*if (tempD <= 0x02 && tempB <= 0x01){
			tempB = PINB & 0x01;
		}*/
		 if ((tempD >= 0x03) && (tempD < 0x23 )){
			tempB = (tempB & 0x00) | 0x04;
		}
		else if (tempD >= 0x23){
			tempB =  (tempB & 0x00) | 0x02;
		}

		PORTB = tempB & 0x06;
	}
	return 1;
}
