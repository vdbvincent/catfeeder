/*
 * alarme.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _ALARME_H_
#define _ALARME_H_

#include "common.h"
#include "ctrleur.h"
#include "menu.h"
#include "clock.h"


typedef struct
{
	clock horaire;
	Bool isValid;
} Alarme_t;


void alarme_setup(void);
void alarme_every10ms(void);
void alarme_every1s(void);

// Méthode permettant de regler une alarme. Retourne 0 en cas d'echec
char alarme_setAlarme(clock p_al, char indice);


#endif /* _ALARME_H_ */