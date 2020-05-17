/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #9  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;

	if (frequency != current_frequency){
		if(!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) { OCR3A = 0xFFFF; }

		else if (frequency > 31250) { OCR3A = 0x0000; }

		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); 
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{Start, Wait, PA0pressed, PA1pressed, PA2pressed, PA0wait, PA1wait, PA2wait}state;
unsigned char tempA;
unsigned char isToggle;
unsigned char i;
double notes[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.0, 493.88, 523.25};

void tick(){
	tempA = ~PINA & 0x07;
	switch (state){
		case Start:
			state = Wait;
			break;

		case Wait:
			if (tempA == 0x01){
				state = PA0pressed;
			}
			else if (tempA == 0x02 && isToggle){
				state = PA1pressed;
			}	
			else if (tempA == 0x04 && isToggle){
				state = PA2pressed;
			}
			else{
				state = Wait;
			}
			break;

		case PA0pressed:
			if (tempA == 0x01)
				state = PA0wait;
			else
				state = Wait;
			break;

		case PA1pressed:
			if (tempA  == 0x02)
				state = PA1wait;
			else
				state = Wait;
			break;

		case PA2pressed:
			if (tempA == 0x04)
				state = PA2wait;
			else
				state = Wait;
			break;
		
		case PA0wait:
			if (tempA == 0x01)
				state = PA0wait;
			else
				state = Wait;
			break;
		
		case PA1wait:
			if (tempA == 0x02)
				state = PA1wait;
			else
				state = Wait;
			break;
		
		case PA2wait:
			if (tempA == 0x04)
				state = PA2wait;
			else
				state = Wait;
			break;

		default:
			break;
	}

	switch (state){
		case Start:
			break;

		case Wait:
			if(isToggle)
				set_PWM(notes[i]);
			else
				set_PWM(0);
			break;

		case PA0pressed:
			isToggle = !isToggle;
			if(!isToggle)
				i = 0;
			break;

		case PA1pressed:
			if(i < 7)
				i++;
			break;

		case PA2pressed:
			if(i > 0)
				i--;
			break;

		case PA0wait:
			break;

		case PA1wait:
			break;

		case PA2wait:
			break;

	}
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	PWM_on();
	tempA = 0x00;
	state = Start;
	isToggle = 0;
	i = 0;

	while (1) {
		tick();
	}

	PWM_off();

	return 1;
}
