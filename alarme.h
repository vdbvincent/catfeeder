/*
 * alarme.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _ALARME_H_
#define _ALARME_H_

#include "common.h"
#include "clock.h"
#include "logs.h"

// Nombre max d'alarme configurable
#define MAX_COUNT_ALARM 5
// Nombre de minuterie configurable - Estimer la meilleur taille dans le rapport utilité/taille utilisée
#define MAX_COUNT_MINUT 5

// Structure d'une alarme
typedef struct Alarme_t Alarme_t;
struct Alarme_t
{
	clock horaire;           // structure contenant l'heure de déclenchement
	void (*foncteur)(void);  // Callback à appeler à l'échéance
	struct Alarme_t * suivant;
};

// Définition du type linked list
//typedef Alarme_t * llist;

// Structure d'un minuteur
typedef struct
{
	uint16_t delai;          // Délai de la minuterie en 100ene de millisecondes
	void (*foncteur)(void);  // Callback à appeler à l'échéance
} Minuteur_t;


void alarme_setup(void);
void alarme_every100ms(void);
void alarme_every1mn(void);

// Méthode permettant de régler une alarme. Retourne 0 en cas d'échec
char alarme_setAlarme(clock p_al, void (*callback)(void));
// Méthode permettant de régler une minuterie. Retourne 0 en cas d'échec
// delai en secondes
// callback
char alarme_setMinuteur(uint16_t p_delai, void (*callback)(void));

// Fonction permettant de retourner un pointeur sur la premiere alarme de la liste chainée
Alarme_t * alarme_getAlarme(void);


// Fonction permettant de supprimer une alarme
Bool alarme_delAlarme(uint8_t p_selection);

#endif /* _ALARME_H_ */
