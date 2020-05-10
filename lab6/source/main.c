/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link : https://drive.google.com/file/d/1k88fjfiPs06BBSg25nxl5v9J_5zIvjd9/view?usp=drivesdk
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

enum States{Start, PB0on, PB1on, PB2on} state;
unsigned char tmpB;

void Tick(){
	switch(state){
		case Start:
			state = PB0on;
			break;

		case PB0on:
			state = PB1on;
			break;

		case PB1on:
			state = PB2on;
			break;

		case PB2on:
			state = PB0on;
			break;

		default:
			break;
	}

	switch(state){
		case Start:
			break;

		case PB0on:
			tmpB = 0x01;
			break;

		case PB1on:
			tmpB = 0x02;
			break;

		case PB2on:
			tmpB = 0x04;
			break;

		default:
			break;
	}
}

int main(void) {
	DDRB = 0xFF; // Set PORTB to output
	PORTB = 0x00; // Init port B to 0s
	TimerSet(1000);
	TimerOn();
	tmpB = 0x00;
	state = Start;

    	while (1) {
		/*tmpB = ~tmpB; // Toggle PORTB; Temporary, bad programming style
		PORTB = tmpB;*/
		Tick();
		while(!TimerFlag); // Wait 1 sec
		TimerFlag = 0;
		PORTB = tmpB;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
    	}
	return 1;	
}
