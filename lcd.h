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
#define SELECT_BACK    0
#define SELECT_OK      1
#define SELECT_CANCEL  2
#define NO_SELECT      99

// Structure d'un menu
typedef struct
{
  char * titre;   // Titre du menu
  char ** items;  // Tableau de choix du menu
  uint8_t nbItem; // Nombre de choix possible
} Menu_t;

// Structure d'un selection
typedef struct
{
  unsigned char selection;
  unsigned char retour;
} Select_t;

// Creation du symbole reveil
byte reveil1[8] = 
{
    B0011,
    B0010,
    B0100,
    B1101,
    B1110,
    B0010,
    B0001,
    B0000
};
byte reveil2[8] = 
{
    B1111,
    B0000,
    B0001,
    B0101,
    B0100,
    B0111,
    B0001,
    B1110
};
byte reveil3[8] = 
{
    B1000,
    B1000,
    B0100,
    B0110,
    B1110,
    B1000,
    B0000,
    B0000
};

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
Select_t afficheMenu(Menu_t * myMenu, uint8_t select = 255);

// Affichage du symbole de reveil
void afficheReveil(void);

// Gestion du popup
void lcd_popup(const char * p_msg);
void procPopup(void); // callback

#endif /* _LCD_H_ */