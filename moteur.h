/*
 * moteur.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#ifndef _MOTEUR_H_
#define _MOTEUR_H_
 
#include "common.h"
#include "ctrleur.h"



void moteur_setup(void);
void moteur_every1ms(void);

void clockwiserotate(void);
void counterclockwiserotate(void);
void step1(void);
void step2(void);
void step3(void);
void step4(void);
void step5(void);
void step6(void);
void step7(void);
void step8(void);
void step0(void);

void sendMoteurEvent(char event);

#endif /* _MOTEUR_H_ */
