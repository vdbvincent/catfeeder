/*
 * boutons.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _BOUTONS_H_
#define _BOUTONS_H_

#include "mcubind.h"
#include "common.h"
#include "ctrleur.h"

typedef struct
{
	char state;
    char retEtat;
    int icpt;
} CTXT_FILTRE;


/*******************
*       LOG        *
********************/
#define LOG_PRINTF
#ifdef LOG_PRINTF
	log(value)	printstr(##value)
#else
#endif


void boutons_setup(int p_delayFiltre_ms);
void boutons_every10ms(void);

char lireBouton(uint8_t voie);

char lireBoutonHaut(void);
char lireBoutonBas(void);
char lireBoutonDroit(void);
char lireBoutonGauche(void);

void sendEvent(char event);


#endif /* _BOUTONS_H_ */
