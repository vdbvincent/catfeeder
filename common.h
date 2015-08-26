/*
 * common.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#define NO_EVENT    -1

// Définition des événements boutons
#define NON_PRESSE  0
#define ENFONCE     1
#define PRESSE      2

#define AUCUN_EVENEMENT   0
#define EVENEMENT_PRESSE  1
#define EVENEMENT_RELACHE 2

#define BT_G_PRESSE  0
#define BT_G_RELACHE 1
#define BT_D_PRESSE  2
#define BT_D_RELACHE 3
#define BT_H_PRESSE  4
#define BT_H_RELACHE 5
#define BT_B_PRESSE  6
#define BT_B_RELACHE 7

// Définition du type bool
typedef enum 
{ False = 0, True = 1}
Bool;

// Définition de la structure clock
typedef struct
{
  char heures;
  char minutes;
  char secondes;
}clock;


// Définition des événements moteur
#define FEDD1  1
#define FEED2  2
#define FEED3  3
#define BEGIN  4
#define END    5

#endif /* _COMMON_H_ */
