/*
 * clock.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _CLOCK_H_
#define _CLOCK_H_

//#include "ctrleur.h"
//#include "menu.h"

#include "common.h"
#include "logs.h"

void clock_setup(void);
void clock_every100ms(void);

clock clock_getClock(void);
void clock_setClock(clock p_myclock);
void clock_reset(void);
uint8_t clock_isInit(void);
void clock_adjust(clock p_myclock);
void procCtmp(void);

void clock_maj(void);
void clock_idle(void);

#endif /* _CLOCK_H_ */