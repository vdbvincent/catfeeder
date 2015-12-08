/*
 * mcubind.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "mcubind.h"


// DEFINITIONS

// Vitesse du port série
#define BAUD_RATE 115200


void init_mcubind(void)
{
	init_serial();
	init_timer1();
	adc_init();
}


/***************************************************************************/
/*                                                                         */
/*    __201008110080__  Liaison série                                      */
/*                                                                         */
/***************************************************************************/
static void init_serial(void)
{
	// Calcul valeur du registre
	#if BAUD_RATE < 57600
		// Basse vitesse
		uint16_t UBRR0_value = ((F_CPU / (8L * BAUD_RATE)) - 1) / 2;
		UCSR0A &= ~(1 << U2X0); // doubleur vitesse off
	#else
		// haute vitesse
		uint16_t UBRR0_value = ((F_CPU / (4L * BAUD_RATE)) - 1) / 2;
		UCSR0A |= (1 << U2X0); // doubleur vitesse on
	#endif

	// Ecriture des registres en 2 fois
	UBRR0H = UBRR0_value >> 8;
	UBRR0L = UBRR0_value;

	// Activer l'émission
	UCSR0B |= 1 << TXEN0;

	// Le reste est en valeur par défaut
	// 8 bit, pas de parité, 1 bit de stop
}

// Envoi d'un caractère
static void putchr(char c)
{
	// On attend que ce soit dispo/utilisable
	loop_until_bit_is_set(UCSR0A, UDRE0);
	// Ecriture registre = envoi de l'octet
	UDR0 = c;
}

// Envoi d'une chaine de caractères
static void printstr(const char * s)
{
	// On boucle tant qu'il y a des caractères
	while(*s)
	{
	    // on change LF et CR si necessaire
	    if (*s == '\n')
	    	putchr('\r');
	    putchr(*s++);
	}
}


/***************************************************************************/
/*                                                                         */
/*    __201008110080__  TIMER 1                                            */
/*                                                                         */
/***************************************************************************/

static void init_timer1(void)
{
	cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    OCR1A = 7812;//   0.000064  // 10ms de periode
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);

    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    every500ms(); // Appel du sequenceur
}



/***************************************************************************/
/*                                                                         */
/*    __201008110080__  mcubind_virtualport_init()                         */
/*                                                                         */
/***************************************************************************/

void mcubind_virtualport_init(uint8_t virtualPort, uint8_t flagUseAsOutput)
{
	if (flagUseAsOutput)
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_B_b0 : DDRB |= _BV(PB0) ; break ;
			case MCUBIND_VIRTUALPORT_B_b1 : DDRB |= _BV(PB1) ; break ;
			case MCUBIND_VIRTUALPORT_B_b2 : DDRB |= _BV(PB2) ; break ;
			case MCUBIND_VIRTUALPORT_B_b3 : DDRB |= _BV(PB3) ; break ;
			case MCUBIND_VIRTUALPORT_B_b4 : DDRB |= _BV(PB4) ; break ;
			case MCUBIND_VIRTUALPORT_B_b5 : DDRB |= _BV(PB5) ; break ;
			case MCUBIND_VIRTUALPORT_B_b6 : DDRB |= _BV(PB6) ; break ;
			case MCUBIND_VIRTUALPORT_B_b7 : DDRB |= _BV(PB7) ; break ;
		}
	}
	else
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_B_b0 : DDRB &= ~_BV(PB0) ; break ;
			case MCUBIND_VIRTUALPORT_B_b1 : DDRB &= ~_BV(PB1) ; break ;
			case MCUBIND_VIRTUALPORT_B_b2 : DDRB &= ~_BV(PB2) ; break ;
			case MCUBIND_VIRTUALPORT_B_b3 : DDRB &= ~_BV(PB3) ; break ;
			case MCUBIND_VIRTUALPORT_B_b4 : DDRB &= ~_BV(PB4) ; break ;
			case MCUBIND_VIRTUALPORT_B_b5 : DDRB &= ~_BV(PB5) ; break ;
			case MCUBIND_VIRTUALPORT_B_b6 : DDRB &= ~_BV(PB6) ; break ;
			case MCUBIND_VIRTUALPORT_B_b7 : DDRB &= ~_BV(PB7) ; break ;
		}
	}
}

/***************************************************************************/
/*                                                                         */
/*    __201008110090__  mcubind_virtualport_read()                         */
/*                                                                         */
/***************************************************************************/

uint8_t mcubind_virtualport_read(uint8_t virtualPort)
{
	switch (virtualPort)
	{
		case MCUBIND_VIRTUALPORT_ADC00 : return ((adc_lecture(0) * 5) / 249);  // make convertion
		case MCUBIND_VIRTUALPORT_ADC01 : return ((adc_lecture(1) * 5) / 249);
		case MCUBIND_VIRTUALPORT_ADC02 : return ((adc_lecture(2) * 5) / 249);
		case MCUBIND_VIRTUALPORT_ADC03 : return ((adc_lecture(3) * 5) / 249);
		case MCUBIND_VIRTUALPORT_ADC04 : return ((adc_lecture(4) * 5) / 249);
		case MCUBIND_VIRTUALPORT_ADC05 : return ((adc_lecture(5) * 5) / 249);
		case MCUBIND_VIRTUALPORT_ADC06 : return ((adc_lecture(6) * 5) / 249);
	}

}



/***************************************************************************/
/*                                                                         */
/*    __201008110100__  mcubind_virtualport_write()                        */
/*                                                                         */
/***************************************************************************/

void mcubind_virtualport_write(uint8_t virtualPort, uint8_t value)
{
	if (value)
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_B_b0 : PORTB |= _BV(PB0) ; break ;
			case MCUBIND_VIRTUALPORT_B_b1 : PORTB |= _BV(PB1) ; break ;
			case MCUBIND_VIRTUALPORT_B_b2 : PORTB |= _BV(PB2) ; break ;
			case MCUBIND_VIRTUALPORT_B_b3 : PORTB |= _BV(PB3) ; break ;
			case MCUBIND_VIRTUALPORT_B_b4 : PORTB |= _BV(PB4) ; break ;
			case MCUBIND_VIRTUALPORT_B_b5 : PORTB |= _BV(PB5) ; break ;
			case MCUBIND_VIRTUALPORT_B_b6 : PORTB |= _BV(PB6) ; break ;
			case MCUBIND_VIRTUALPORT_B_b7 : PORTB |= _BV(PB7) ; break ;
		}
	}
	else
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_B_b0 : PORTB &= ~_BV(PB0) ; break ;
			case MCUBIND_VIRTUALPORT_B_b1 : PORTB &= ~_BV(PB1) ; break ;
			case MCUBIND_VIRTUALPORT_B_b2 : PORTB &= ~_BV(PB2) ; break ;
			case MCUBIND_VIRTUALPORT_B_b3 : PORTB &= ~_BV(PB3) ; break ;
			case MCUBIND_VIRTUALPORT_B_b4 : PORTB &= ~_BV(PB4) ; break ;
			case MCUBIND_VIRTUALPORT_B_b5 : PORTB &= ~_BV(PB5) ; break ;
			case MCUBIND_VIRTUALPORT_B_b6 : PORTB &= ~_BV(PB6) ; break ;
			case MCUBIND_VIRTUALPORT_B_b7 : PORTB &= ~_BV(PB7) ; break ;
		}
	}
}

/***************************************************************************/
/*                                                                         */
/*    __201008110040__  adc_init()                                         */
/*                                                                         */
/***************************************************************************/

static void adc_init(void)
{
	// For Aref=AVcc;
	ADMUX=(1<<REFS0);
	//Prescalar div factor = 128
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

/***************************************************************************/
/*                                                                         */
/*    __201008110040__  adc_lecture()                                      */
/*                                                                         */
/***************************************************************************/

static uint8_t adc_lecture(uint8_t voie)
{
	// choose channel voie must be 0-7
	//voie &= 0x03;
	ADMUX |= voie & 0x03;

	// Start single conversion
	ADCSRA |= (1 << ADSC);

	// Wait for convertion to complete
	while (! (ADCSRA & ( 1 << ADIF) ) );
	
	// Clear ADIF by writting 1 to it
	ADCSRA |= (1 << ADIF);

	return ADCW;
}