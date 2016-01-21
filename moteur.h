/*
 * moteur.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#ifndef _MOTEUR_H_
#define _MOTEUR_H_
 
#include "common.h"
#include "logs.h"

// TODO : rendre ces valeurs configurable et les stocker en EEPROM
#define MT_PETIT   2	// Temps de fonctionnement pour la petite part en secondes
#define MT_MOYEN   4
#define MT_GRAND   8
#define MT_NONE    0

#define DUTYCYCLE  38 // Rapport cyclique 38 = 15% de 255

void moteur_setup(void);
void moteur_every100ms(void);

Bool moteur_setCmd(uint8_t value);

#endif /* _MOTEUR_H_ */