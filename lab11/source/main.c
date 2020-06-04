/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "scheduler.h"
#include "io.c"
#endif

enum GameOnSM_States {g_Init, g_Wait, g_Press, g_Wait2, g_Release};
unsigned char isStart = 0x00;

int gameOnTick(int state){
	unsigned char tempB = ~PINB & 0x01;
	
	// State transitions
	switch(state){
		case g_Init:
			state = g_Wait;
			break;

		case g_Wait:
			if(tempB){
				state = g_Press;
			}
			else{
				state = g_Wait;
			}
			break;

		case g_Press:
			state = g_Wait2;
			break;
		
		case g_Wait2:
			if(tempB){
				state = g_Wait2;
			}
			else{
				state = g_Release;
			}
			break;

		case g_Release:
			state = g_Wait;
			break;

		default:
			break;
	}

	// State actions
	switch(state){
		case g_Init:
			break;

		case g_Wait:
			break;

		case g_Press:
			break;

		case g_Wait2:
			break;

		case g_Release:
			isStart = ~isStart;
			break;
		
		default:
			break;
	}
	//PORTA = isStart;
	return state;
}

enum playerMoveSMStates {p_Init, p_Wait, p_Move};
unsigned char charPos = 2;

int playerMoveTick(int state){
	unsigned char up = ~PINB & 0x02;
	unsigned char down = ~PINB & 0x04;

	// State transitions
	switch (state){
		case p_Init:
			state = p_Wait;
			break;

		case p_Wait:
			if(isStart){
				state = p_Move;
			}
			else{
				state = p_Wait;
			}
			break;

		case p_Move:
			if(!isStart){
				state = p_Wait;
			}
			break;

		default:
			break;
	}

	// State actions
	switch (state){
		case p_Init:
			break;

		case p_Wait:
			break;

		case p_Move:
			if(up){
				charPos = 2;
			}
			else if (down){
				charPos = 18;
			}
			break;

		default:
			break;
	}
	//PORTA = up; 
	return state;
}

enum obstacleSMStates{o_Init, o_Wait, o_Scroll};
unsigned char top = 9;
unsigned char bot = 30;
unsigned char i = 0;

int obstacleTick(int state){
	switch(state){
		case o_Init:
			state = o_Wait;
			break;

		case o_Wait:
			if(isStart){
				state = o_Scroll;
			}
			else{
				state = o_Wait;
			}
			break;

		case o_Scroll:
			if (isStart){
				state = o_Scroll;
			}
			else{
				state = o_Wait;
			}
			break;

		default:
			break;
	}

	// State actions
	switch(state){
		case o_Init:
			break;

		case o_Wait:
			break;

		case o_Scroll:
			if(top == 1){
				top = 16;
			}
			if(bot == 17){
				bot = 32;
			}
			LCD_ClearScreen();
			LCD_Cursor(top);
			LCD_WriteData('*');
			LCD_Cursor(bot);
			LCD_WriteData('*');
			top--;
			bot--;
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
			/*if(isStart){
				LCD_DisplayString(1, "Start");
			}
			else{
				LCD_DisplayString(1, "Pause");
			}*/
			LCD_Cursor(charPos);
			break;

		default: 
			state = d_Init;
			break;
	}

	return state;
}
int main(void) {
	DDRA = 0xff; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xff;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	static task task1, task2, task3, task5;
	task *tasks[] ={&task1, &task2, &task3, &task5};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;
	// Task 1 keypadTask
	task1.state = g_Init;
	task1.period = 200;
	task1.elapsedTime = task1.period;
	task1.TickFct = &gameOnTick;

	// Task 2 displayTask
	task2.state = p_Init;
	task2.period = 200;
	task2.elapsedTime = task2.period;
	task2.TickFct = &playerMoveTick;
	
	// Task 3 obstacleTask
	task3.state = o_Init;
	task3.period = 200;
	task3.elapsedTime = task3.period;
	task3.TickFct = &obstacleTick;

	// Task 5 displayTask
	task5.state = start;
	task5.period = 200;
	task5.elapsedTime = task5.period;
	task5.TickFct = &displayTick;
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
