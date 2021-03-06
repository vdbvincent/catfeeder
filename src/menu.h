/*
 * menu.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#ifndef _MENU_H_
#define _MENU_H_
 
#include "common.h"
#include "clock.h"
#include "alarme.h"
#include "boutons.h"
#include "lcd.h"
#include "moteur.h"

// Définitions des actions MENU
#define MENU_OK         0
#define MENU_NO_ACTION  1
#define MENU_CANCEL     2
#define MENU_EVENT      0
#define MENU_NO_EVENT   4

void menu_setup(uint8_t p_tempoDem = 2);
void menu_idle(void);

void menu_affMenu(void);
char giveFood(void);

void procGtmp(void);


void clearCmdButtons(void);


// Méthode permettant de configurer un objet clock
// Si aucun param, le structure globale horloge est utilisé et valorisé par la clock
// Si une clock est passée en param, elle servira
char setAclock(clock p_clock);
char setAnAlarm(void);
// Permet de remettre à 0 les champs de la structure horloge globale
void clearHorloge(void);

// Méthode pour eteindre ou allumer l'écran
void menu_on(void);
void menu_off(void);
void menu_setMenuOff(void);
void menu_SetMenuOn(void);


#endif /* _MENU_H_ */
