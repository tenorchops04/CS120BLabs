/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #3
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
#include "io.c"
#include "bit.h"
#endif

enum keypadSM_States {kSM_Init, kSM_Wait, kSM_Press, kSM_Wait2, kSM_Release};
unsigned char tempB = '\0';
unsigned char isPressed = 0;

int keypadTick(int state){
	unsigned char tempC = GetKeypadKey();
	
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
			state = kSM_Wait2;
			break;

		case kSM_Wait2:
			if(tempC == tempB){
				state = kSM_Wait2;
			}
			else{
				state = kSM_Release;
			}
			break;

		case kSM_Release:
			state = kSM_Wait;
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
			isPressed = 0;
			break;
			
		case kSM_Press:
			tempB = tempC;
			break;

		case kSM_Wait2:
			break;

		case kSM_Release:
			isPressed = 1;
			break;
	}

	return state;
}

enum displaySMStates {d_Init, d_Display};

int displayTick(int state){
	switch(state){
		case d_Init:
			state = d_Display;
			break;

		case d_Display:
			state = d_Display;
			break;

		default:
			state = d_Init;
			break;
	}

	switch(state){
		case d_Init:
			break;

		case d_Display:
			if(isPressed){
				LCD_Cursor(1);
				LCD_WriteData(tempB);
			}
			break;

		default: 
			state = d_Init;
			break;
	}

	return state;
}

int main(void) {
	DDRA = 0xF0; PORTA = 0xF0;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTA = 0x00;
	
	static task task1, task2;
	task *tasks[] ={&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;
	// Task 1 keypadTask
	task1.state = start;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadTick;

	// Task 2 displayTask
	task2.state = start;
	task2.period = 200;
	task2.elapsedTime = task2.period;
	task2.TickFct = &displayTick;

	TimerSet(200);
	TimerOn();

	LCD_init();

	unsigned short i;
	while (1) {
		for (i = 0; i <numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 200;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
