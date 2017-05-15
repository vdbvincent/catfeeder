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
char * SSALARM_MENU_ITEMS[8];

Menu_t SSALARM_MENU = 
{
  (char *)"Regler alarme",
  SSALARM_MENU_ITEMS,
  6
};


// Déclaration du menu principal
char * MAIN_MENU_ITEMS[] = 
{
  (char *)"Regler horloge",
  (char *)"Regler alarme",
  (char *)"Donner a manger"
};

Menu_t MAIN_MENU = 
{
  (char *)"Menu principal",
  MAIN_MENU_ITEMS,
  3
};

// Déclaration du menu Donner a manger
char * FEED_MENU_ITEMS[] =
{
  (char *)"1/2 portion",
  (char *)"1 portion",
  (char *)"2 portions"
};
Menu_t FEED_MENU = 
{
  (char *)"Choisir quantite",
  FEED_MENU_ITEMS,
  3
};

// Déclaration du menu reglage heure
char * CLOCK_HOUR_MENU_ITEMS[] =
{
  (char *)"00",
  (char *)"01",
  (char *)"02",
  (char *)"03",
  (char *)"04",
  (char *)"05",
  (char *)"06",
  (char *)"07",
  (char *)"08",
  (char *)"09",
  (char *)"10",
  (char *)"11",
  (char *)"12",
  (char *)"13",
  (char *)"14",
  (char *)"15",
  (char *)"16",
  (char *)"17",
  (char *)"18",
  (char *)"19",
  (char *)"20",
  (char *)"21",
  (char *)"22",
  (char *)"23"
};
char * CLOCK_MINSEC_MENU_ITEMS[] =
{
  (char *)"00",
  (char *)"01",
  (char *)"02",
  (char *)"03",
  (char *)"04",
  (char *)"05",
  (char *)"06",
  (char *)"07",
  (char *)"08",
  (char *)"09",
  (char *)"10",
  (char *)"11",
  (char *)"12",
  (char *)"13",
  (char *)"14",
  (char *)"15",
  (char *)"16",
  (char *)"17",
  (char *)"18",
  (char *)"19",
  (char *)"20",
  (char *)"21",
  (char *)"22",
  (char *)"23",
  (char *)"24",
  (char *)"25",
  (char *)"26",
  (char *)"27",
  (char *)"28",
  (char *)"29",
  (char *)"30",
  (char *)"31",
  (char *)"32",
  (char *)"33",
  (char *)"34",
  (char *)"35",
  (char *)"36",
  (char *)"37",
  (char *)"38",
  (char *)"39",
  (char *)"40",
  (char *)"41",  
  (char *)"42",
  (char *)"43",
  (char *)"44",
  (char *)"45",
  (char *)"46",
  (char *)"47",
  (char *)"48",
  (char *)"49",
  (char *)"50",
  (char *)"51",
  (char *)"52",
  (char *)"53",
  (char *)"54",
  (char *)"55",
  (char *)"56",
  (char *)"57",
  (char *)"58",
  (char *)"59"
};
Menu_t CLOCK_HOUR_MENU =
{
  (char *)"Heure :",
  CLOCK_HOUR_MENU_ITEMS,
  24
};
Menu_t CLOCK_MIN_MENU =
{
  (char *)"Minute :",
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

  // Allocation dynamique pour le tableau d'alarmes
  SSALARM_MENU_ITEMS[0] = (char*)malloc(8);
  SSALARM_MENU_ITEMS[1] = (char*)malloc(6);
  SSALARM_MENU_ITEMS[2] = (char*)malloc(6);
  SSALARM_MENU_ITEMS[3] = (char*)malloc(6);
  SSALARM_MENU_ITEMS[4] = (char*)malloc(6);
  SSALARM_MENU_ITEMS[5] = (char*)malloc(6);
  strcpy(SSALARM_MENU_ITEMS[0], "Ajouter");
  strcpy(SSALARM_MENU_ITEMS[1], "");
  strcpy(SSALARM_MENU_ITEMS[2], "");
  strcpy(SSALARM_MENU_ITEMS[3], "");
  strcpy(SSALARM_MENU_ITEMS[4], "");
  strcpy(SSALARM_MENU_ITEMS[5], "");

  // Init de la sortie d'allimentation de l'écran en sortie
  mcubind_virtualport_init(MCUBIND_VIRTUALPORT_D_b4, 1);
  mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b4, 1);
}

Bool lcd_is_init(void)
{
  return lcd_isInit;
}

void lcd_on(void)
{
  lcd.clear();
  // Allumer le backlight
  mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b4, 1);
  // Allumer le lcd
  lcd.display();
}

void lcd_off(void)
{
  // Eteindre le backlight
  mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b4, 0);
  // Eteindre le lcd
  lcd.noDisplay();
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
  if (lcd_isInit && (m_bPopup == False))
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
    // Afficher le menu désiré
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
