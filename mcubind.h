/*
 * mcubind.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#ifndef _MCUBIND_H_
#define _MCUBIND_H_

#include "common.h"
#include "catfeeder.h"

/***************************************************************************/
/*                                                                         */
/*    __201006210030__  wrapper de virtualisation                          */
/*                                                                         */
/*    partie est propre au microcontrôleur et aux périphériques utilisés   */
/*                                                                         */
/***************************************************************************/
/*
#ifdef FAKEDEV
	#define 	F_CPU		16000000
	#define 	UCSR0A	uint8_t unused_value0
	#define  UBRR0H   uint8_t unused_value1
	#define  UBRR0L   uint8_t unused_value2
	#define  UCSR0B   uint8_t unused_value3
	#define  U2X0     uint8_t unused_value4
#endif
*/

/***************************************************************************/
/*                                                                         */
/*    __201006210030__  #define MCUBIND_VIRTUALPORT_X_bY                   */
/*                                                                         */
/*    partie est propre au microcontrôleur et aux périphériques utilisés   */
/*                                                                         */
/***************************************************************************/

// entrées ou sorties directes tout ou rien

#define MCUBIND_VIRTUALPORT_D_b0                0x00
#define MCUBIND_VIRTUALPORT_D_b1                0x01
#define MCUBIND_VIRTUALPORT_D_b2                0x02
#define MCUBIND_VIRTUALPORT_D_b3                0x03
#define MCUBIND_VIRTUALPORT_D_b4                0x04
#define MCUBIND_VIRTUALPORT_D_b5                0x05
#define MCUBIND_VIRTUALPORT_D_b6                0x06
#define MCUBIND_VIRTUALPORT_D_b7                0x07
#define MCUBIND_VIRTUALPORT_B_b8                0x08
#define MCUBIND_VIRTUALPORT_B_b9                0x09
#define MCUBIND_VIRTUALPORT_B_b10               0x10
#define MCUBIND_VIRTUALPORT_B_b11               0x11
#define MCUBIND_VIRTUALPORT_B_b12               0x12
#define MCUBIND_VIRTUALPORT_B_b13               0x13



// entrées directes analogiques

#define MCUBIND_VIRTUALPORT_ADC00               0x44
#define MCUBIND_VIRTUALPORT_ADC01               0x45
#define MCUBIND_VIRTUALPORT_ADC02               0x46   // 5V_SYS, PAD02/AN02 pin 53
#define MCUBIND_VIRTUALPORT_ADC03               0x47
#define MCUBIND_VIRTUALPORT_ADC04               0x48   // BAT12V_P, PAD04/AN04 pin 55
#define MCUBIND_VIRTUALPORT_ADC05               0x49
#define MCUBIND_VIRTUALPORT_ADC06               0x4A

// sorties directes pwm

#define MCUBIND_VIRTUALPORT_PWM00               0x5C
#define MCUBIND_VIRTUALPORT_PWM01               0x5D
#define MCUBIND_VIRTUALPORT_PWM02               0x5E
#define MCUBIND_VIRTUALPORT_PWM03               0x5F
#define MCUBIND_VIRTUALPORT_PWM04               0x60
#define MCUBIND_VIRTUALPORT_PWM05               0x61
#define MCUBIND_VIRTUALPORT_PWM06               0x62

/***************************************************************************/
/*                                                                         */
/*    __201006210050__  prototypes                                         */
/*                                                                         */
/***************************************************************************/

// Init
void init_mcubind(void);

// Timers
static void init_timer1(void);

// Virtualisation des IO
void mcubind_virtualport_init(uint8_t virtualPort, uint8_t flagUseAsOutput) ;
uint16_t mcubind_virtualport_read(uint8_t virtualPort) ;
void mcubind_virtualport_write(uint8_t virtualPort, uint8_t value) ;

// Debug serie
static void init_serial(void);
static void putchr(char c);
void printstr(const char * s);

// ADC
static void adc_init(void);
static uint16_t adc_lecture(uint8_t voie);

// PWM
static void pwm_init(void);
static void pwm_out(uint8_t pwm);

#endif /* _MCUBIND_H_ */