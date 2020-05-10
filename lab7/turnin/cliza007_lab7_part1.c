/*	Author: Celvin Lizama Pena
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/file/d/1e0y-gCYJ721H0jdWmRhAerO271xQOHfs/view?usp=drivesdk
 */

#include <avr/io.h>
#include "io.h"
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

enum States{Start, Wait, PA0pressed, PA0wait, PA1pressed, PA1wait, Reset}state;

unsigned char count;
unsigned char timecnt;

void Tick(){
	unsigned char tmpA = ~PINA & 0x03;
	// State transitions
	switch(state){
		case Start:
			state = Wait;
		break;

		case Wait:
			if(!tmpA){
				state = Wait;
			}
			else if(tmpA == 0x01){
				state = PA0pressed;
			}
			else if(tmpA == 0x02){
				state = PA1pressed;
			}
			else if(tmpA == 0x03){
				state = Reset;
			}
		break;

		case PA0pressed:
			if(!tmpA){
				state = Wait;
			}
			else if(tmpA == 0x01){
				state = PA0wait;
			}
			else if(tmpA == 0x02){
				state = PA1pressed;
			}
			else if(tmpA == 0x03){
				state = Reset;
			}
		break;

		case PA0wait:
			if(!tmpA){
				state = Wait;
			}
			else if(tmpA == 0x01){
				state = PA0wait;
			}
			else if(tmpA == 0x02){
				state = PA1pressed;
			}
			else if(tmpA == 0x03){
				state = Reset;
			}
		break;

		case PA1pressed:
			if(!tmpA){
				state = Wait;
			}
			else if(tmpA == 0x01){
				state = PA0pressed;
			}
			else if(tmpA == 0x02){
				state = PA1wait;
			}
			else if(tmpA == 0x03){
				state = Reset;
			}
		break;

		case PA1wait:
			if(!tmpA){
				state = Wait;
			}
			else if(tmpA == 0x01){
				state = PA0pressed;
			}
			else if(tmpA == 0x02){
				state = PA1wait;
			}
			else if(tmpA == 0x03){
				state = Reset;
			}
		break;
	
		case Reset:
			if(!tmpA){
				state = Wait;
			}
			else if(tmpA == 0x01){
				state = PA0pressed;
			}
			else if(tmpA == 0x02){
				state = PA1pressed;
			}
			else if(tmpA == 0x03){
				state = Reset;
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
		break;
		
		case PA0wait:
			if(timecnt <10){
				timecnt++;
			}
			else{
				timecnt = 0;
				if(count<9){
					count++;
				}
			}
		break;

		case PA1pressed:
			if(count > 0){
				count--;
			}
		break;

		case PA1wait:
			if(timecnt <10){
				timecnt++;
			}
			else{
				timecnt = 0;
				if(count>0){
					count--;
				}
			}
		break;

		case Reset:
			count = 0;
		break;

		default:
		break;
	}
		LCD_ClearScreen();
		LCD_WriteData(count + '0');
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	TimerSet(100);
	TimerOn();

	count = 0x00;
	state = Start;
	timecnt = 0x00;

	LCD_init();
	LCD_WriteData(count + '0');
	
    	while (1) {
		Tick();
		while(!TimerFlag); // Wait 1 sec
		TimerFlag = 0;
    	}
	return 1;	
}
