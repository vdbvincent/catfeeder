/*
 * mcubind.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#ifndef _MCUBIND_H_
#define _MCUBIND_H_

#include "catfeeder.h"


/***************************************************************************/
/*                                                                         */
/*    __201006210030__  #define MCUBIND_VIRTUALPORT_X_bY                   */
/*                                                                         */
/*    partie est propre au microcontrôleur et aux périphériques utilisés   */
/*                                                                         */
/***************************************************************************/

// entrées ou sorties directes tout ou rien

#define MCUBIND_VIRTUALPORT_B_b0                0x00
#define MCUBIND_VIRTUALPORT_B_b1                0x01
#define MCUBIND_VIRTUALPORT_B_b2                0x02
#define MCUBIND_VIRTUALPORT_B_b3                0x03
#define MCUBIND_VIRTUALPORT_B_b4                0x04
#define MCUBIND_VIRTUALPORT_B_b5                0x05
#define MCUBIND_VIRTUALPORT_B_b6                0x06
#define MCUBIND_VIRTUALPORT_B_b7                0x07


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
static void init_serial(void);
static void init_timer1(void);

void mcubind_virtualport_init(uint8_t virtualPort, uint8_t flagUseAsOutput) ;
uint8_t mcubind_virtualport_read(uint8_t virtualPort) ;
void mcubind_virtualport_write(uint8_t virtualPort, uint8_t value) ;

static void putchr(char c);
static void printstr(const char * s);
static void adc_init(void);
static uint8_t adc_lecture(uint8_t voie);


#endif /* _MCUBIND_H_ */