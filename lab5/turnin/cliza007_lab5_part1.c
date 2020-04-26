/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: none
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * 
 * 	Demo: https://drive.google.com/file/d/17m5cqTe0FdPghGyZNW-_p9gvqdyufhzd/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char tempA = 0x00;
	unsigned char tempC = 0x00;

	/* Insert your solution below */
	while (1) {

		tempA = ~PINA & 0x0F;

		if (tempA == 0){
			 tempC = (tempC & 0x00)| 0x40;
		}
		else if (tempA == 1 || tempA == 2){
			tempC = (tempC & 0x00) | 0x60;
		}
		else if (tempA <= 4){
			tempC = (tempC & 0x00) | 0x70;
		}
		else if (tempA <= 6){
			tempC = (tempC & 0x00) | 0x38;
		}
		else if (tempA <= 9){
			tempC = (tempC & 0x00) | 0x3C;
		}
		else if (tempA <= 12){
			tempC = (tempC & 0x00) | 0x3E;
		}
		else if (tempA <= 15){
			tempC = (tempC & 0x00) | 0x3F;  
		}

		PORTC = tempC;	
	}
	return 1;
}
