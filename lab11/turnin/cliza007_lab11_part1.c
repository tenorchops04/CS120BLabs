/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "io.h"
#include "bit.h"
#endif


enum keypadSM_States {kSM_Init, kSM_Wait, kSM_Press };
unsigned char tempC = '\0';
unsigned char tempB;

int keypadTick(int state){
	tempC = GetKeypadKey();

	// State transitions
	switch(state){
		case kSM_Init:
			state = kSM_Wait;
			break;

		case kSM_Wait:
			if(tempC != '\0'){
				state = kSM_Press;
			}
			else{
				state = kSM_Wait;
			}
			break;

		case kSM_Press:
			if(tempC == (tempB + '0')){
				state = kSM_Press;
			}
			else{
				state = kSM_Wait;
			}
			break;

		default:
			state = kSM_Wait;
			break;
	}
	// State actions
	switch(state){
		case kSM_Init:
			break;

		case kSM_Wait:
			break;
			
		case kSM_Press:
			switch(tempC){
				case '\0': tempB = 0x1F; break;
				case '1': tempB = 0x01; break;
				case '2': tempB = 0x02; break;
				case '3': tempB = 0x03; break;
				case '4': tempB = 0x04; break;
				case '5': tempB = 0x05; break;
				case '6': tempB = 0x06; break;
				case '7': tempB = 0x07; break;
				case '8': tempB = 0x08; break;
				case '9': tempB = 0x09; break;
				case 'A': tempB = 0x0A; break;
				case 'B': tempB = 0x0B; break;
				case 'C': tempB = 0x0C; break;
				case 'D': tempB = 0x0D; break;
				case '*': tempB = 0x0E; break;
				case '0': tempB = 0x00; break;
				case '#': tempB = 0x0F; break;
				default: tempB = 0x1B;
			}
			break;
	}

	PORTB = tempB;

	return state;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0xF0; PORTA = 0xF0;
	
	static task task1;
	task *tasks[] ={&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	// Task 1 keypadTick
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadTick;

	TimerSet(50);
	TimerOn();

	unsigned short i;
	while (1) {
		for (i = 0; i <numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 50;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
