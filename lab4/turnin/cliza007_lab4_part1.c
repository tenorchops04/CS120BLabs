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

enum States{Start, PB0on, PB0release, PB1on, PB1release}state;

unsigned char isButtonPressed;
unsigned char tempB;

void Tick(){
	// State transitions
	switch(state){
		case Start:
			state = PB0release;
		break;

		case PB0on:
			if(isButtonPressed){
				state = PB0on;
			}
			else{
				state = PB0release;
			}
		break;

		case PB0release:
			if(isButtonPressed){
				state = PB1on;
			}
			else{
				state = PB0release;
			}
		break;

		case PB1on:
			if(isButtonPressed){
				state = PB1on;
			}
			else{
				state = PB1release;
			}
		break;

		case PB1release:
			if(isButtonPressed){
				state = PB0on;
			}
			else{
				state = PB1release;
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
		
		case PB0release:
		break;

		case PB1on:
			tempB = (tempB & 0x00) | 0x02;
		break;

		case PB1release:
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
	tempB = 0x01;
	state = Start;

	/* Insert your solution below */
	while (1) {
		isButtonPressed = PINA & 0x01;
		
		Tick();

		PORTB = tempB;
	}
	return 1;
}
