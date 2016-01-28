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
void clock_setClock(clock * p_myclock);
inline void clock_reset(void);
inline uint8_t clock_isInit(void);

#endif /* _CLOCK_H_ */