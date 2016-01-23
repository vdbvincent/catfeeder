/*
 * alarme.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _ALARME_H_
#define _ALARME_H_

#include "common.h"
//#include "menu.h"
#include "clock.h"
#include "logs.h"


// Structure d'une alarme
typedef struct
{
	clock horaire;           // structure contenant l'heure de déclenchement
	void (*foncteur)(void);  // Callback à appeler à l'échéance
} Alarme_t;

// Structure d'un minuteur
typedef struct
{
	uint32_t delai;          // Délai de la minuterie en 100ene de millisecondes
	void (*foncteur)(void);  // Callback à appeler à l'échéance
} Minuteur_t;


void alarme_setup(void);
void alarme_every100ms(void);
void alarme_every1mn(void);

// Méthode permettant de régler une alarme. Retourne 0 en cas d'échec
char alarme_setAlarme(clock p_al, uint8_t pos, void (*callback)(void));
// Méthode permettant de régler une minuterie. Retourne 0 en cas d'échec
// delai en secondes
// callback avec @ d'un char a mettre à 1
char alarme_setMinuteur(uint32_t p_delai, void (*callback)(void));

// Fonction permettant de retourner l'alarme à l'indice donnée
char * alarme_getAlarme(uint8_t p_selection);

#endif /* _ALARME_H_ */