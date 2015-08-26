/*
 * boutons.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _BOUTONS_H_
#define _BOUTONS_H_

#include "common.h"
#include "ctrleur.h"

void boutons_setup(int p_delayFiltre_ms);
void boutons_every10ms(void);

char lireBoutonHaut(void);
char lireBoutonBas(void);
char lireBoutonDroit(void);
char lireBoutonGauche(void);

void sendEvent(char event);


#endif /* _BOUTONS_H_ */
