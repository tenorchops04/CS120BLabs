/*	Author: Celvin Lizama Pena 
 *  Partner(s) Name: none 
 *	Lab Section: 022
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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

unsigned char i;
unsigned char j;
static unsigned char threeLEDs;
static unsigned char blinkingLED;
static unsigned char combineLEDs;

enum TL_States {TL_SMStart, TL_S1} TL_state;
void TickFct_ThreeLeds(){
	unsigned char leds[] = {0x01, 0x02, 0x04};

	// State transitions
	switch (TL_state){
		case TL_SMStart:
			TL_state = TL_S1;
			break;

		case TL_S1:
			TL_state = TL_S1;
			break;	

		default:
			break;
	}

	// State actions
	switch(TL_state){
		case TL_SMStart:
			i = 0;
			break;

		case TL_S1:
			if (i == 3){
				i = 0;
			}
			threeLEDs = leds[i % 3];
			i++;
			break;

		default:
			break;
	}	
}

enum BL_States { BL_SMStart, BL_S1 } BL_state;
void TickFct_BlinkLed(){
	unsigned char leds[] = {0x08, 0x00};

	// State transitions
	switch(BL_state){
		case BL_SMStart:
			BL_state = BL_S1;
			break;

		case BL_S1:
			BL_state = BL_S1;
			break;

		default:
			break;

	}

	// State transitions
	switch(BL_state){
		case BL_SMStart:
			j = 0;
			break;

		case BL_S1:
			if (j == 2){
				j = 0;
			}
			blinkingLED = leds[j % 2];
			j++;
			break;

		default:
			break;
	}
}

enum CL_States {CL_SMStart, CL_S1} CL_state;
void Tick_CombineLEDs(){
	// State transitions
	switch(CL_state){
		case CL_SMStart:
			CL_state = CL_S1;
			break;

		case CL_S1:
			CL_state = CL_S1;
			break;

		default:
			break;
	}

	// State actions
	switch(CL_state){
		case CL_SMStart:
			break;

		case CL_S1:
			combineLEDs = 0xFF & (blinkingLED | threeLEDs);
			break;

		default:
			break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned long BL_elapsedTime = 1000;
	unsigned long TL_elapsedTime = 300;
	unsigned long CL_elapsedTime = 100;
	unsigned long timerPeriod = 100;
	
	TimerSet(timerPeriod);
	TimerOn();

	BL_state = BL_SMStart;
	TL_state = TL_SMStart;
	CL_state = CL_SMStart;

	while (1) {
		if(BL_elapsedTime >= 1000){	
			TickFct_BlinkLed();
			BL_elapsedTime = 0;
		}
		if(TL_elapsedTime >= 300){
			TickFct_ThreeLeds();
			TL_elapsedTime = 0;
		}
		if(CL_elapsedTime >= 100){
			Tick_CombineLEDs();
			CL_elapsedTime = 0;
		}

		while(!TimerFlag);
		TimerFlag = 0;

		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
		CL_elapsedTime += timerPeriod;

		PORTB = combineLEDs;
	}
	return 1;
}
