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

enum States{Start, PB0on, PB0pressed, PB1on, PB1pressed}state;

unsigned char isButtonPressed;
unsigned char tempB;

void Tick(){
	// State transitions
	switch(state){
		case Start:
			state = PB0on;
		break;

		case PB0on:
			if(isButtonPressed){
				state = PB0pressed;
			}
			else{
				state = PB0on;
			}
		break;

		case PB0pressed:
			if(isButtonPressed){
				state = PB0pressed;
			}
			else{
				state = PB1on;
			}
		break;

		case PB1on:
			if(isButtonPressed){
				state = PB1pressed;
			}
			else{
				state = PB1on;
			}
		break;

		case PB1pressed:
			if(isButtonPressed){
				state = PB1pressed;
			}
			else{
				state = PB0on;
			}
		break;

		default:
		break;
	}

	// State actions
	switch(state){
		case PB0on:
			tempB = (tempB & 0x00) | 0x01;
		break;
		
		case PB0pressed:
		break;

		case PB1on:
			tempB = (tempB & 0x00) | 0x02;
		break;

		case PB1pressed:
		break;

		default:
		break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xff;
	DDRB = 0xff; PORTB = 0x00;

	isButtonPressed = 0x00;
	tempB = 0x00;
	state = Start;

	/* Insert your solution below */
	while (1) {
		isButtonPressed = PINA & 0x01;
		
		Tick();

		PORTB = tempB;
	}
	return 1;
}
