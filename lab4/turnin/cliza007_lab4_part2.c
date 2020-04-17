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

enum States{Start, Wait, PA0pressed, PA0wait, PA1pressed, PA1wait, Reset}state;

unsigned char count;

void Tick(){
	// State transitions
	switch(state){
		case Start:
			state = Wait;
		break;

		case Wait:
			if(!PINA){
				state = Wait;
			}
			else if(PINA == 0x01){
				state = PA0pressed;
			}
			else if(PINA == 0x02){
				state = PA1pressed;
			}
			else if(PINA == 0x03){
				state = Reset;
			}
		break;

		case PA0pressed:
			if(!PINA){
				state = Wait;
			}
			else if(PINA == 0x01){
				state = PA0wait;
			}
			else if(PINA == 0x02){
				state = PA1pressed;
			}
			else if(PINA == 0x03){
				state = Reset;
			}
		break;

		case PA0wait:
			if(!PINA){
				state = Wait;
			}
			else if(PINA == 0x01){
				state = PA0wait;
			}
			else if(PINA == 0x02){
				state = PA1pressed;
			}
			else if(PINA == 0x03){
				state = Reset;
			}
		break;

		case PA1pressed:
			if(!PINA){
				state = Wait;
			}
			else if(PINA == 0x01){
				state = PA0pressed;
			}
			else if(PINA == 0x02){
				state = PA1wait;
			}
			else if(PINA == 0x03){
				state = Reset;
			}
		break;

		case PA1wait:
			if(!PINA){
				state = Wait;
			}
			else if(PINA == 0x01){
				state = PA0pressed;
			}
			else if(PINA == 0x02){
				state = PA1wait;
			}
			else if(PINA == 0x03){
				state = Reset;
			}
		break;
	
		case Reset:
			if(!PINA){
				state = Wait;
			}
			else if(PINA == 0x01){
				state = PA0pressed;
			}
			else if(PINA == 0x02){
				state = PA1pressed;
			}
			else if(PINA == 0x03){
				state = Reset;
			}
		break;

		default:
		break;
	}

	// State actions
	switch(state){
		case Wait:
		break;

		case PA0pressed:
			if(count < 9){
				count++;
			}
		break;
		
		case PA0wait:
		break;

		case PA1pressed:
			if(count > 0){
				count--;
			}
		break;

		case PA1wait:
		break;

		case Reset:
			count = 0;
		break;

		default:
		break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xff;
	DDRC = 0xff; PORTC = 0x00;

	count = 0x07;
	state = Start;

	/* Insert your solution below */
	while (1){
		Tick();

		PORTC = count;
	}
	return 1;
}
