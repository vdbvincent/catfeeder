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
#include "logs.h"

#include "QueueArray.h"


// Définition de la fifo
QueueArray<char>   f_boutons;

// Fonctions de base
void boutons_setup(int p_delayFiltre_ms);
void boutons_every100ms(void);

// Fonctions internes
static uint16_t lireBouton(void);
static char lireEvenement(int * p_bouton);

// Fonctions de fifo
static void sendEvent(char event);
static void put_btfifo(char event);
Bool isEmpty_btfifo(void);
char get_btfifo(void);
Bool isFull_btfifo(void);

uint8_t boutons_getBPCounter(void);

#endif /* _BOUTONS_H_ */