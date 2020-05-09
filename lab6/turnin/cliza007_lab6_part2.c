/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/file/d/1VZ2hRcpZnNEkkz7DTGFyLKGmL8V4UZGM/view?usp=drivesdk
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

enum States{Start, Run1, Press, Release, Run2} state;
unsigned char tmpA;
unsigned char tmpB;
unsigned char leds[] = {0x01, 0x02, 0x04, 0x02};
unsigned char i;

void Tick(){
	tmpA = !(PINA & 0x01);
	switch(state){
		case Start:
			state = Run1;
			break;

		case Run1:
			if(tmpA == 1){
				state = Press;
			}
			else{
				state = Run1;
			}
			break;

		case Press:
			if (tmpA == 1){
				state = Press;
			}
			else{
				state = Release;
			}
			break;

		case Release:
			if (tmpA == 1){
				state = Run2;
			}
			else{
				state = Release;
			}
			break;

		case Run2:
			if(tmpA == 1){
				state = Run2;
			}
			else{
				state = Run1;
			}
			break;

		default:
			break;
	}

	switch(state){
		case Start:
			break;

		case Run1:
			i++;
			tmpB = leds[i%4];
			break;

		case Press:
			break;

		case Release:
			i = -1;
			break;

		case Run2:
			i++;
			tmpB = leds[i%4];
			break;
		
		default:
			break;
	}
	PORTB = tmpB;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; // Set PORTB to output
	PORTB = 0x00; // Init port B to 0s
	
	TimerSet(300);
	TimerOn();

	tmpB = 0x00;
	state = Start;
	i = -1;
	
    	while (1) {
		Tick();
		while(!TimerFlag); // Wait 1 sec
		TimerFlag = 0;
    	}
	return 1;	
}
