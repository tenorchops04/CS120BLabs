/*	Author: cliza007
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo: https://drive.google.com/file/d/1sMnIOEP0CY641DOzpaTW_rg29NSn2Qee/view?usp=sharing
 */
#include <avr/io.h>
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait, PA0pressed, PA0wait, PA1pressed, PA1wait, Reset, Rwait}state;

unsigned char count;
unsigned char tempA;

void Tick(){
	// State transitions
	switch(state){
		case Start:
			state = Wait;
		break;

		case Wait:
			if(tempA == 0x00){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0pressed;
			}
			else if(tempA == 0x02){
				state = PA1pressed;
			}
			else if(tempA == 0x03){
				state = Reset;
			}
		break;

		case PA0pressed:
			if(!tempA){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0wait;
			}
			else if(tempA == 0x02){
				state = PA1pressed;
			}
			else if(tempA == 0x03){
				state = Reset;
			}
		break;

		case PA0wait:
			if(!tempA){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0wait;
			}
			else if(tempA == 0x02){
				state = PA1pressed;
			}
			else if(tempA == 0x03){
				state = Reset;
			}
		break;

		case PA1pressed:
			if(!tempA){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0pressed;
			}
			else if(tempA == 0x02){
				state = PA1wait;
			}
			else if(tempA == 0x03){
				state = Reset;
			}
		break;

		case PA1wait:
			if(!tempA){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0pressed;
			}
			else if(tempA == 0x02){
				state = PA1wait;
			}
			else if(tempA == 0x03){
				state = Reset;
			}
		break;
	
		case Reset:
			if(!tempA){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0pressed;
			}
			else if(tempA == 0x02){
				state = PA1pressed;
			}
			else if(tempA == 0x03){
				state = Rwait;
			}
		break;

		case Rwait:
			if(!tempA){
				state = Wait;
			}
			else if(tempA == 0x01){
				state = PA0pressed;
			}
			else if(tempA == 0x02){
				state = PA1pressed;
			}
			else if(tempA == 0x03){
				state = Rwait;
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
			LCD_ClearScreen();
			LCD_WriteData(count + '0');
		break;
		
		case PA0wait:
		break;

		case PA1pressed:
			if(count > 0){
				count--;
			}
			LCD_ClearScreen();
			LCD_WriteData(count + '0');
		break;

		case PA1wait:
		break;

		case Reset:
			count = 0;
			LCD_ClearScreen();
			LCD_WriteData(count + '0');
		break;
		
		case Rwait:
		break;

		default:
		break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	LCD_init();
	LCD_ClearScreen();
	LCD_WriteData('0');

	tempA = 0x00;	
	count = 0x00;
	state = Start;
	
	/* Insert your solution below */
	while (1){
		tempA = ~PINA & 0x03;
		Tick();
		
		//PORTB = tempA;
		//PORTC = count;

	}

	return 1;
}
