/*
 * lcd.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#ifndef _LCD_H_
#define _LCD_H_
 
#include "common.h"
#include "clock.h"
#include "alarme.h"

// Définitions
#define SELECT_BACK    (uint8_t)0
#define SELECT_OK      (uint8_t)1
#define SELECT_CANCEL  (uint8_t)2
#define NO_SELECT      (uint8_t)99

// Structure d'un menu
typedef struct
{
  char * titre;   // Titre du menu
  char ** items;  // Tableau de choix du menu
  uint8_t nbItem; // Nombre de choix possible
} Menu_t;

// Structure d'un menu alarme
typedef struct
{
  char * titre;   // Titre du menu
  char * items[6][8];  // Tableau de choix du menu
  uint8_t nbItem; // Nombre de choix possible
} Menu_al_t;

// Structure d'un selection
typedef struct
{
  uint8_t selection;
  uint8_t retour;
} Select_t;


// Déclaration du menu principal
extern char * MAIN_MENU_ITEMS[];
extern Menu_t MAIN_MENU;

// Déclaration du menu Donner a manger
extern char * FEED_MENU_ITEMS[];
extern Menu_t FEED_MENU;

// Déclaration du menu reglage heure
extern char * CLOCK_HOUR_MENU_ITEMS[];
extern char * CLOCK_MINSEC_MENU_ITEMS[];
extern Menu_t CLOCK_HOUR_MENU;
extern Menu_t CLOCK_MIN_MENU;
extern Menu_t CLOCK_SEC_MENU ;

// Déclaration du menu alarme
extern char * ALARME_MENU_ITEMS[];
extern Menu_t ALARME_MENU;

extern char * SSALARM_MENU_ITEMS[6];
//extern char * SSALARM_MENU_ITEMS[];
extern Menu_t SSALARM_MENU;

void lcd_setup(void);
void lcd_every10ms(void);

Bool lcd_is_init(void);
void lcd_clear(void);

// Affiche l'écrand de démarrage
void welcomeScreen(void);

// Affiche de l'écran d'acceuil
void afficheHome(uint8_t forced = 0);
void afficheHeure(uint8_t forced = 0);
//void afficheTempsRestant(uint8_t forced = 0);
void afficheBtMenu(uint8_t forced = 0);

// Affichage du menu
// Select : param optionnel par defaut mettre 255
Select_t afficheMenu(Menu_t * myMenu, uint8_t select);

// Gestion du popup
void lcd_popup(const char * p_msg);
void procPopup(void); // callback

// Méthodes pour eteindre ou allumer l'écran
void lcd_on(void);
void lcd_off(void);

#endif /* _LCD_H_ */
