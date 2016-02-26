/*
* logs.h
*
* Created on: 31 juil. 2015
* Author: vincent
*/
#ifndef _LOGS_H_
#define _LOGS_H_

#include "common.h"
#include "mcubind.h"

#define MAX_SIZE_LOG	64  // Taille max d'un log

#define FILTRE  3 // Filtrage de l'affichage des logs. 0 = Rien afficher
//													   1 - Afficher les logs ERREUR
//													   2 - Afficher les logs ERREUR + INFO
//													   3 - Afficher les logs ERREUR + INFO + DEBUG


typedef struct
{
	char crit;
	char texte[MAX_SIZE_LOG];
} Logs_t;


// Déclarations
void logs_setup(void);
void logs_idle(void);

// Seule fonction visible de l'extérieur
void print_log(char crit, /*char **/const __FlashStringHelper * texte);


#endif /* _LOGS_H_ */