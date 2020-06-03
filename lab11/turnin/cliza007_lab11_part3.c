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
#include "io.c"
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
				case '\0': tempB = ' '; break;
				case '1': tempB = '1'; break;
				case '2': tempB = '2'; break;
				case '3': tempB = '3'; break;
				case '4': tempB = '4'; break;
				case '5': tempB = '5'; break;
				case '6': tempB = '6'; break;
				case '7': tempB = '7'; break;
				case '8': tempB = '8'; break;
				case '9': tempB = '9'; break;
				case 'A': tempB = 'A'; break;
				case 'B': tempB = 'B'; break;
				case 'C': tempB = 'C'; break;
				case 'D': tempB = 'D'; break;
				case '*': tempB = '*'; break;
				case '0': tempB = '0'; break;
				case '#': tempB = '#'; break;
				default: tempB = 0x1B;
			}
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
			if(tempC != '\0'){
				state = d_Display;
			}
			else{
				state = d_Init;
			}
			break;

		default:
			state = d_Init;
			break;
	}

	switch(state){
		case d_Init:
			break;

		case d_Display:
			LCD_ClearScreen();
			if(tempB != '\0')
				LCD_WriteData(tempB);
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