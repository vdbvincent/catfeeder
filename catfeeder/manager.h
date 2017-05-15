/*
 * manager.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "common.h"
#include "logs.h"
#include "clock.h"
#include "boutons.h"

#define ALLUMER_MENU    0
#define ATTENTE_BP      1
#define SURVEILLANCE_BP 2
#define SOMMEIL         3

void manager_setup(void);
void manager_every100ms(void);

void manager_setAlarme(clock p_horloge);

void manager_procAlarme(void);
void manager_procMinut(void);


#endif /* _MANAGER_H_ */