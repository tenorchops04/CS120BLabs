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

enum States{Start,Locked, PA2pressed, PA2released, Unlocked}state;

unsigned char tempB;
unsigned char arr[4] = {0x04, 0x01, 0x02, 0x01};
unsigned char i;

void Tick(){
	// State transitions
	switch(state){
		case Start:
			state = Locked;
		break;

		case Locked:
			if(PINA == arr[i]){
				state = PA2pressed;
			}
			else{
				state = Locked;
			}
		break;

		case PA2pressed:
			if(!PINA){
				state = PA2released;
				i++;
			}
			else if(PINA == arr[i]){
				state = PA2pressed;
			}
			else{
				state = Locked;
			}
		break;

		case PA2released:
			if(!PINA){
				state = PA2released;
			}
			else if(i < 3 && PINA == arr[i]){
				state = PA2pressed;
			}
			else if(i ==  3 && PINA == arr[i]){
				if(tempB == 0x00){	
					state = Unlocked;
				}
				else{
					state = Locked;
				}
			}
			else{
				state = Locked;
			}
		break;

		case Unlocked:
			if(PINA == 0x80){
				state = Locked;
			}
			else if(PINA == arr[i]){
				state = PA2pressed;
			}
			else{
				state = Unlocked;
			}
		break;

		default:
		break;
	}

	// State actions
	switch(state){
		case Locked:
			tempB = 0x00;
			i = 0x00;
		break;

		case PA2pressed:
		break;
		
		case PA2released:
		break;

		case Unlocked:
			tempB = 0x01;
			i = 0x00;
		break;

		default:
		break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xff;
	DDRB = 0xff; PORTB = 0x00;

	i = 0x00;
	tempB = 0x00;
	state = Start;

	/* Insert your solution below */
	while (1){
		Tick();

		PORTB = tempB;
	}
	return 1;
}
