/*
 * clock.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "common.h"
#include "ctrleur.h"
#include "menu.h"

void clock_setup(void);
void clock_every10ms(void);
void clock_every1ms(void);

clock clock_getClock(void);
void clock_setClock(clock p_myclock);
void clock_reset(void);

#endif /* _CLOCK_H_ */