/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to out cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0, Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks.

void TimerOn(){
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; // bit 3 = 0: CTC mode (clear timer on compare)
		       // bit2bit1bit0=011: pre-scaler /64
		       // 00001011: 0x0B
		       // SO, 8 MHz clock or 8,000,000 / 64 = 125,000 ticks/s
		       // Thus, TCNT1 register will count at 125,000 ticks/s
		       
	// AVR output compares register OCR1A.
	OCR1A = 125; // Timer interrupt will be generated when TCN1==OCR1A
		     // We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
		     // So when TCNT1 register equals 125,
		     // 1 ms has passed. Thus, we compare to 125.

	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	// Initialize avr counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	
	// Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff(){
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR(){
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect){
	
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	
	if(_avr_timer_cntcurr == 0){ //results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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

enum States{Start, Wait, Play, Wait2}state;
unsigned char tempA;
unsigned char isDone;
unsigned char i;
unsigned char count;
/*D4 = 293.66;
E4 = 329.63;
F4 = 349.23;
G4 = 392.00;
A4 = 440.00;
B4 = 493.88;
C5 = 523.25;
D5 = 587.33;
E5 = 659.25;
rest = 0;*/

double notes[] = {349.23, 440.00, 493.88, 349.23, 440.00, 493.88, 349.23, 440.00, 493.88, 659.25, 587.33, 493.88, 523.25, 493.88, 392.00, 329.63, 0, 293.66, 329.63, 392.00, 329.63, 0};	
double value[] = {1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 6, 2, 1, 1, 1, 4, 4};

void tick(){
	tempA = ~PINA & 0x01;
	switch (state){
		case Start:
			state = Wait;
			break;

		case Wait:
			if (tempA == 0x01){
				state = Play;
			}
			break;

		case Play:
			if (!isDone)
				state = Play;
			else
				state = Wait2;
			break;
		case Wait2:
			if(tempA == 0x01)
				state = Wait2;
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
			isDone = 0;
			i = 0;
			count  = 0;
			break;

		case Play:
			if(i == 22)
				isDone = 1;
			else{
				if(count < value[i]){
					set_PWM(notes[i]);
					count++;
				}
				else{
					count = 0;
					i++;
				}
			}
			break;

		default:
			break;
	}
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	PWM_on();
	TimerSet(110);
	TimerOn();

	tempA = 0x00;
	state = Start;
	isDone = 0;
	i = 0;
	count = 0;
	
	while (1) {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}

	PWM_off();

	return 1;
}
