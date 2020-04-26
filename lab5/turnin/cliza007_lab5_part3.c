/*	Author: cliza007
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo: https://drive.google.com/file/d/1r-2HPrLls5jp2Xch5c2uF2vNcq6sWHIq/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait1, Lit1, Lit2, Wait2, Pair1, Wait3, Pair2, Wait4, Pair3, Wait5, AllLit, WaitAll}state;

unsigned char button;
unsigned char tempC;

void Tick(){
	// State transitions
	switch(state){
		case Start:
			state = Wait1;
		break;

		case Wait1:
			if(button){
				state = Lit2;
			}
			else{
				state = Wait1;
			}
		break;
	
		case Lit2:
			if(button){
				state = Lit2;
			}
			else{
				state = Wait2;
			}
		break;

		case Wait2:
			if(button){
				state = Pair1;
			}
			else{
				state = Wait2;
			}
		break;

		case Pair1:
			if(button){
				state = Pair1;
			}
			else{
				state = Wait3;
			}
		break;

		case Wait3:
			if(button){
				state = Pair2;
			}
			else{
				state = Wait3;
			}
		break;

		case Pair2:
			if(button){
				state = Pair2;
			}
			else{
				state = Wait4;
			}
		break;

		case Wait4:
			if(button){
				state = Pair3;
			}
			else{
				state = Wait4;
			}
		break;

		case Pair3:
			if(button){
				state = Pair3;
			}
			else{
				state = Wait5;
			}
		break;
		
		case Wait5:
			if(button){
				state = AllLit;
			}
			else{
				state = Wait5;
			}
		break;

		case AllLit:
			if(button){
				state = AllLit;
			}
			else{
				state = WaitAll;
			}
		break;

		case WaitAll:
			if(button){
				state = Lit1;
			}
			else{
				state = WaitAll;
			}
		break;
	
		case Lit1:
			if(button){
				state = Lit1;
			}
			else{
				state = Wait1;
			}
		break;

		default:
		break;
	}

	// State actions
	switch(state){
		case Wait1:
		break;

		case Lit2:
			tempC = 0xAA;
		break;
		
		case Wait2:
		break;

		case Pair1:
			tempC = 0x0C;
		break;
		
		case Wait3:
		break;
		
		case Pair2:
			tempC = 0x12;
		break;

		case Wait4:
		break;

		case Pair3:
			tempC = 0x21;
		break;

		case Wait5:
		break;

		case AllLit:
			tempC = 0xFF;
		break;

		case WaitAll:
		break;

		case Lit1:
			tempC = 0x55;
		break;

		default:
		break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	button = 0x00;	
	tempC = 0x55;
	state = Start;

	/* Insert your solution below */
	while (1){
		button = ~PINA & 0x01;
		Tick();
		
		//PORTB = tempA;
		PORTC = tempC;
	}
	return 1;
}
