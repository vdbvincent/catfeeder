/*
 * boutons.h
 *
 * Created on : 27 janv. 2017
 * Author : vincent
 *
*/

#ifndef _BOUTONS_H_
#define _BOUTONS_H_


#include "mcubind.h"
#include "common.h"
#include "logs.h"


typedef struct
{
	char state;
    char retEtat;
    uint8_t pin;
    int icpt;
} CTXT_BP;
 


// Fonctions de base
void boutons_setup(uint16_t p_delayFiltre_ms);
void boutons_every100ms(void);

// Fonctions internes
static void lireEvenement(uint8_t indice);  // Lire un evenement bouton selon un contexte


// Fonctions de fifo
static void sendEvent(char event);
static void put_btfifo(char event);
Bool isEmpty_btfifo(void);
char get_btfifo(void);
Bool isFull_btfifo(void);

uint8_t boutons_getBPCounter(void);

#endif /* _BOUTONS_H_ */