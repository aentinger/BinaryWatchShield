/**
 * @author Alexander Entinger, MSc / LXRobotics
 * @brief this module implements the library for controlling the LXRobotics Binary Watch Shield
 * @file LXR_Binary_Watch_Shield.cpp
 * @license Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 4.0) ( https://creativecommons.org/licenses/by-nc-sa/4.0/ )
 */
 
#include "LXR_Binary_Watch_Shield.h"

/* C CODE */

/* INCLUDES */

#include <avr/io.h>
#include <avr/interrupt.h>

/* DEFINES */

// DIGIT0 = D13 = PB5
#define DIGIT_0_DDR		(DDRB)
#define DIGIT_0_PORT	(PORTB)
#define DIGIT_0_bm		(1<<PORTB5)
// DIGIT1 = D11 = PB3
#define DIGIT_1_DDR		(DDRB)
#define DIGIT_1_PORT	(PORTB)
#define DIGIT_1_bm		(1<<PORTB3)
// DIGIT2 = D6 = PD6
#define DIGIT_2_DDR		(DDRD)
#define DIGIT_2_PORT	(PORTD)
#define DIGIT_2_bm		(1<<PORTD6)
// DIGIT3 = D3 = PD3
#define DIGIT_3_DDR		(DDRD)
#define DIGIT_3_PORT	(PORTD)
#define DIGIT_3_bm		(1<<PORTD3)
// BIT0 = D12 = PB4
#define BIT_0_DDR		(DDRB)
#define BIT_0_PORT		(PORTB)
#define BIT_0_bm		(1<<PORTB4)
// BIT1 = D4 = PD4
#define BIT_1_DDR		(DDRD)
#define BIT_1_PORT		(PORTD)
#define BIT_1_bm		(1<<PORTD4)
// BIT2 = D7 = PD7
#define BIT_2_DDR		(DDRD)
#define BIT_2_PORT		(PORTD)
#define BIT_2_bm		(1<<PORTD7)
// BIT3 = D5 = PD5
#define BIT_3_DDR		(DDRD)
#define BIT_3_PORT		(PORTD)
#define BIT_3_bm		(1<<PORTD5)

/* TYPEDEFS */

typedef enum {DIGIT0 = 0, DIGIT1 = 1, DIGIT2 = 2, DIGIT3 = 3} E_DIGIT_SELECT;

typedef struct 
{
	uint8_t current_digit;
	uint8_t display_data[4];
} s_binary_watch_shield_data; 

/* GLOBAL CONSTANTS */

static uint16_t const TIMER1_RELOAD_VALUE = 0xC180;

/* GLOBAL VARIABLES */

static s_binary_watch_shield_data bws_data = {0};

/* PROTOTYPES */

void set_digit(E_DIGIT_SELECT const digit, uint8_t const val);

/* FUNCTIONS */

/** 
 * @brief initializes the binary watch shield
 */
void init_binary_watch_shield()
{
	// set all pins as outputs
	DIGIT_0_DDR |= DIGIT_0_bm;
	DIGIT_1_DDR |= DIGIT_1_bm;
	DIGIT_2_DDR |= DIGIT_2_bm;
	DIGIT_3_DDR |= DIGIT_3_bm;
	
	BIT_0_DDR |= BIT_0_bm;
	BIT_1_DDR |= BIT_1_bm;
	BIT_2_DDR |= BIT_2_bm;
	BIT_3_DDR |= BIT_3_bm;
	
	// set up timer 1
	// fclk = 16 MHz, prescaler = 1, fTimer = 16 MHz, tTimer = 62.5 ns
	// tUpdate,Period = 1 ms -> 16000 Timersteps
	// Reload-Wert = 65536 - 16000 = 0xC180
	TCNT1 = TIMER1_RELOAD_VALUE;
	// enable overflow interrupt
	TIMSK1 = (1<<TOIE1);
	// start timer
	TCCR1B = (1<<CS10);	
}

/** 
 * @brief set the numbers for the binary watch shield to display
 */
void binary_watch_shield_set_display(uint8_t const digit_3, uint8_t const digit_2, uint8_t const digit_1, uint8_t const digit_0)
{
	cli();
	bws_data.display_data[DIGIT0] = digit_0;
	bws_data.display_data[DIGIT1] = digit_1;
	bws_data.display_data[DIGIT2] = digit_2;
	bws_data.display_data[DIGIT3] = digit_3;
	sei();
}

void set_digit(E_DIGIT_SELECT const digit, uint8_t const val)
{
	// switch all digits off
	DIGIT_0_PORT &= ~DIGIT_0_bm;
	DIGIT_1_PORT &= ~DIGIT_1_bm;
	DIGIT_2_PORT &= ~DIGIT_2_bm;
	DIGIT_3_PORT &= ~DIGIT_3_bm;
	
	// switch the selected digit on
	switch(digit)
	{
		case DIGIT0: DIGIT_0_PORT |= DIGIT_0_bm; break;		
		case DIGIT1: DIGIT_1_PORT |= DIGIT_1_bm; break;
		case DIGIT2: DIGIT_2_PORT |= DIGIT_2_bm; break;
		case DIGIT3: DIGIT_3_PORT |= DIGIT_3_bm; break;
		default: break;
	}
	
	// display value on the bit lines
	uint8_t tmp = val;
	if(tmp % 2) BIT_0_PORT &= ~BIT_0_bm;
	else BIT_0_PORT |= BIT_0_bm;
	tmp /= 2;
	if(tmp % 2) BIT_1_PORT &= ~BIT_1_bm;
	else BIT_1_PORT |= BIT_1_bm;
	tmp /= 2;
	if(tmp % 2) BIT_2_PORT &= ~BIT_2_bm;
	else BIT_2_PORT |= BIT_2_bm;
	tmp /= 2;
	if(tmp % 2) BIT_3_PORT &= ~BIT_3_bm;
	else BIT_3_PORT |= BIT_3_bm;
}

/** 
 * @brief Interrupt Service Routine for Timer 1 Overflow
 */
ISR(TIMER1_OVF_vect)
{
	// reload timer
	TCNT1 = TIMER1_RELOAD_VALUE;
	
	// set the digit
	set_digit(E_DIGIT_SELECT(bws_data.current_digit), bws_data.display_data[bws_data.current_digit]);
	
	// update current digit state
	bws_data.current_digit = (bws_data.current_digit + 1) % 4;
}

/* CPP WRAPPER */

/** 
 * @brief initializes the library
 */
void LXR_Binary_Watch_Shield::begin()
{
  init_binary_watch_shield();
}
  
/**
 * @brief sets what number should be displayed on which digit of the binary watch shield
 */
void LXR_Binary_Watch_Shield::set_display(uint8_t const digit_3, uint8_t const digit_2, uint8_t const digit_1, uint8_t const digit_0)
{
  binary_watch_shield_set_display(digit_3, digit_2, digit_1, digit_0);
}


