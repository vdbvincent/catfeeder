/*
 * lcd.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#include "LiquidCrystal.h"
#include "lcd.h"
 
LiquidCrystal lcd(11,10,9,8,7,6); //liaison 4 bits de données

// Temps d'affichage d'une popup
static uint8_t m_uiTempoPopup = 2; // 2 secondes
static Bool m_bPopup = False;
// Variable de la FSM
static Bool lcd_isInit = False;

// Déclaration du menu principal
char * MAIN_MENU_ITEMS[] = 
{
  F("Regler horloge"),
  F("Regler alarme"),
  F("Donner a manger")
};

Menu_t MAIN_MENU = 
{
  F("Menu principal"),
  MAIN_MENU_ITEMS,
  3
};

// Déclaration du menu Donner a manger
char * FEED_MENU_ITEMS[] =
{
  F("1/2 portion)",
  F("1 portion)",
  F("2 portions)"
};
Menu_t FEED_MENU = 
{
  F("Choisir quantite"),
  FEED_MENU_ITEMS,
  3
};

// Déclaration du menu reglage heure
char * CLOCK_HOUR_MENU_ITEMS[] =
{
  F("00"),
  F("01"),
  F("02"),
  F("03"),
  F("04"),
  F("05"),
  F("06"),
  F("07"),
  F("08"),
  F("09"),
  F("10"),
  F("11"),
  F("12"),
  F("13"),
  F("14"),
  F("15"),
  F("16"),
  F("17"),
  F("18"),
  F("19"),
  F("20"),
  F("21"),
  F("22"),
  F("23")
};
char * CLOCK_MINSEC_MENU_ITEMS[] =
{
  F("00"),
  F("01"),
  F("02"),
  F("03"),
  F("04"),
  F("05"),
  F("06"),
  F("07"),
  F("08"),
  F("09"),
  F("10"),
  F("11"),
  F("12"),
  F("13"),
  F("14"),
  F("15"),
  F("16"),
  F("17"),
  F("18"),
  F("19"),
  F("20"),
  F("21"),
  F("22"),
  F("23"),
  F("24"),
  F("25"),
  F("26"),
  F("27"),
  F("28"),
  F("29"),
  F("30"),
  F("31"),
  F("32"),
  F("33"),
  F("34"),
  F("35"),
  F("36"),
  F("37"),
  F("38"),
  F("39"),
  F("40"),
  F("41"),  
  F("42"),
  F("43"),
  F("44"),
  F("45"),
  F("46"),
  F("47"),
  F("48"),
  F("49"),
  F("50"),
  F("51"),
  F("52"),
  F("53"),
  F("54"),
  F("55"),
  F("56"),
  F("57"),
  F("58"),
  F("59")
};
Menu_t CLOCK_HOUR_MENU =
{
  F("Heure :"),
  CLOCK_HOUR_MENU_ITEMS,
  24
};
Menu_t CLOCK_MIN_MENU =
{
  F("Minute :"),
  CLOCK_MINSEC_MENU_ITEMS,
  60
};


void lcd_setup(void)
{
  // Chargement du symbole reveil
  //lcd.createChar(0, reveil1);
  //lcd.createChar(1, reveil2);
  //lcd.createChar(2, reveil3);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd_isInit = True;
}

Bool lcd_is_init(void)
{
  return lcd_isInit;
}

void welcomeScreen(void)
{
  if (lcd_isInit)
  {
    lcd.clear();
    lcd.print(F("CatFeeder  v2.0"));
  }
}

void lcd_clear(void)
{
  if (lcd_isInit)
  {
    lcd.clear();
  }
}

void afficheReveil(void)
{
  //lcd.setCursor(0,14);    // Affiche sur la fin de la premiere ligne
  //lcd.write((uint8_t) 0);
  //lcd.write((uint8_t) 1);
  //lcd.write((uint8_t) 2);
}

// Ecran d'acceuil
void afficheHome(uint8_t forced)
{
  afficheHeure(forced);
  //afficheTempsRestant(forced);
  afficheBtMenu(forced);
}

void afficheHeure(uint8_t forced)
{
  static clock oldheure;
  clock heure;
  heure = clock_getClock();
  
  if (forced || (heure.heures != oldheure.heures || heure.minutes != oldheure.minutes))
  {
    // On affiche l'heure seulement si soit c'est forcé soit l'heure à changé
    lcd.setCursor(0,0); // Curseur en haut à gauche
    char message[8];  
    sprintf(message, "%02d:%02d", heure.heures, heure.minutes);
    lcd.print(message);
    oldheure = heure;
  }
}
/*
void afficheTempsRestant(uint8_t forced)
{
  static char * oldtxt;
  char * txt;

  // on  affiche le temps seulement si une alarme est enclenchée
  if (alarme_getAlarme())
  {
    if (forced == 1)
    {
       txt = alarme_getNextAlarmeStr();
       if (strcmp(txt, oldtxt) != 0)
       {
          lcd.setCursor(0,1); // Curseur en haut à gauche
          lcd.print(txt);
          strcpy(oldtxt, txt);
       }
    }
  }
}*/

void afficheBtMenu(uint8_t forced)
{
  if (forced == 1)
  {
    lcd.setCursor(12,1);
    lcd.print(F("menu"));
  }
}


// MENU
Select_t afficheMenu(Menu_t * myMenu , uint8_t select)
{
  static uint8_t selection = 0;
  uint8_t retour = NO_SELECT;
  Select_t ret;
  
  if (select != 255)
  {
    // Si une position est demandée, la prendre
    selection = select;
  }

  if (m_bPopup == True)
  {
    ret.selection = selection;
    ret.retour = retour;
    return ret;
  }
  else
  {
    // Afficher le menu désiré7
    lcd.setCursor(0,0);
    lcd.print(myMenu->titre);
    lcd.setCursor(0,1);
    lcd.print(myMenu->items[selection]);
    
    // Attente d'un événement
    if ( ! isEmpty_btfifo())
    {
      char event = get_btfifo();
      switch(event)
      {
        case BT_B_PRESSE:
          // choix suivant s'il existe
          if (selection < (myMenu->nbItem - 1))
          {
            // Passe au choix suivant
            selection ++;
            // Effacement de la ligne
            lcd.setCursor(0,1);
            lcd.print("                ");
          }
          else
          {
            // Revient au premier choix
            selection = 0;
            // Effacement de la ligne
            lcd.setCursor(0,1);
            lcd.print("                ");
          }
        break;
        
        case BT_H_PRESSE:
          // S'il existe un choix précédent
          if (selection > 0)
          {
            // Passe au choix précédent
            selection --;
            // Effacement de la ligne
            lcd.setCursor(0,1);
            lcd.print("                ");
          }
          else
          {
            // Va au dernier choix
            selection = myMenu->nbItem - 1;
            // Effacement de la ligne
            lcd.setCursor(0,1);
            lcd.print("                ");
          }
        break;
        
        case BT_D_PRESSE:
          // Validation du choix
          retour = SELECT_OK;
          //selection = 0;
        break;
        
        case BT_G_PRESSE:
          // Retour en arriere
          retour = SELECT_CANCEL;;
          selection = 0;
        break;
      }
    }
    
    ret.selection = selection;
    ret.retour = retour;
    
    if (retour == SELECT_OK)
    {
      selection = 0;
    }
    return ret;

  }
}

// GESTION POPUP
void lcd_popup(const char * p_msg)
{
/*
declencher une minuterie de 2s et afficher le texte
bloquer la fonction affichemenu le temps de la popup
et ds la callback tout remettre normal
*/
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(p_msg);
  alarme_setMinuteur((uint16_t)m_uiTempoPopup, &procPopup); // Affiche pour 2 secondes
  m_bPopup = True;
}

void procPopup(void)
{
  lcd.clear();
  m_bPopup = False;
}
