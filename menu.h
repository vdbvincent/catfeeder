/*
 * menu.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#ifndef _MENU_H_
#define _MENU_H_
 
#include "common.h"
#include "lcd.h"
#include "ctrleur.h"
#include "clock.h"

// Définitions des actions MENU
#define MENU_OK         0
#define MENU_NO_ACTION  1
#define MENU_CANCEL     2
#define MENU_EVENT      0
#define MENU_NO_EVENT   4

void menu_setup(void);
void menu_every10ms(void);

void clearCmdButtons(void);

void menu_doFeedTheCatEvent(void);

// Méthode permettant de controler la distribution de nourriture. Retourne 0 lorsque c'est fini. 1 sinon.
char feedTheCat(void);
// Méthode permettant de configurer un objet clock
char setAclock(clock * p_cHorloge);


#endif /* _MENU_H_ */