/*
 * lcd.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#include "LiquidCrystal.h"
#include "lcd.h"
 
LiquidCrystal lcd(11,10,9,8,7,6); //liaison 4 bits de données

// Variable de la FSM
static Bool isInit = False;

// Variables popup
static char popup_timeout = 0;
static char popup = 0;
static char * txt;

// Déclaration du menu principal
char * MAIN_MENU_ITEMS[] = 
{
  "Regler horloge",
  "Regler alarme",
  "Donner a manger"
};

Menu_t MAIN_MENU = 
{
  "Menu principal",
  MAIN_MENU_ITEMS,
  3
};

// Déclaration du menu Donner a manger
char * FEED_MENU_ITEMS[] =
{
  "1/2 portion",
  "1 portion",
  "2 portions"
};
Menu_t FEED_MENU = 
{
  "Choisir quantite",
  FEED_MENU_ITEMS,
  3
};

// Déclaration du menu reglage heure
char * CLOCK_HOUR_MENU_ITEMS[] =
{
  "00",
  "01",
  "02",
  "03",
  "04",
  "05",
  "06",
  "07",
  "08",
  "09",
  "10",
  "11",
  "12",
  "13",
  "14",
  "15",
  "16",
  "17",
  "18",
  "19",
  "20",
  "21",
  "22",
  "23"
};
char * CLOCK_MINSEC_MENU_ITEMS[] =
{
  "00",
  "01",
  "02",
  "03",
  "04",
  "05",
  "06",
  "07",
  "08",
  "09",
  "10",
  "11",
  "12",
  "13",
  "14",
  "15",
  "16",
  "17",
  "18",
  "19",
  "20",
  "21",
  "22",
  "23",
  "24",
  "25",
  "26",
  "27",
  "28",
  "29",
  "30",
  "31",
  "32",
  "33",
  "34",
  "35",
  "36",
  "37",
  "38",
  "39",
  "40",
  "41",  
  "42",
  "43",
  "44",
  "45",
  "46",
  "47",
  "48",
  "49",
  "50",
  "51",
  "52",
  "53",
  "54",
  "55",
  "56",
  "57",
  "58",
  "59"
};
Menu_t CLOCK_HOUR_MENU =
{
  "Heure :",
  CLOCK_HOUR_MENU_ITEMS,
  24
};
Menu_t CLOCK_MIN_MENU =
{
  "Minutes :",
  CLOCK_MINSEC_MENU_ITEMS,
  60
};
Menu_t CLOCK_SEC_MENU =
{
  "Secondes :",
  CLOCK_MINSEC_MENU_ITEMS,
  60
};

// Déclaration du menu alarme
char * ALARME_MENU_ITEMS[] =
{
	"AL 1",
	"AL 2",
	"AL 3",
	"AL 4",
	"AL 5"
};
Menu_t ALARME_MENU =
{
	"Selection alarme",
	ALARME_MENU_ITEMS,
	5
};




void lcd_setup(void)
{
  // Init debug
  Serial.begin(9600);

  // Chargement du symbole reveil
  lcd.createChar(0, reveil1);
  lcd.createChar(1, reveil2);
  lcd.createChar(2, reveil3);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  isInit = True;
}

Bool lcd_init(void)
{
  return isInit;
}

void lcd_popup(char * texte, uint8_t temps)
{
  txt = texte;
  popup_timeout = temps;
  popup = 1;
}


void welcomeScreen(void)
{
  if (isInit)
  {
    lcd.clear();
    lcd.print("CatFeeder  v1.0");
  }
}

void lcd_clear(void)
{
  if (isInit)
  {
    lcd.clear();
    popup_timeout = 0;
  }
}

void afficheReveil(void)
{
	//lcd.setCursor(0,14);		// Affiche sur la fin de la premiere ligne
	//lcd.write((uint8_t) 0);
	//lcd.write((uint8_t) 1);
	//lcd.write((uint8_t) 2);
}

void lcd_every10ms(void)
{
  static int cmp = 0;
  // Gestion d'une popup
  if (popup == 1)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.blink();  // Faire clignoter le curseur
    lcd.print(txt);
    cmp = 0;
    popup = 2;
  }
  else if (popup == 2)
  {
    if (popup_timeout >= 99)
    {
      // temps infini
    }
    else
    {
      if (cmp >= popup_timeout)
      {
        
        lcd.noBlink();
        lcd.clear();
        popup = 0;
      }
      else
      {
        cmp ++;
      }
    }
  }
  
}

// Ecran d'acceuil
void afficheHome(void)
{
  afficheTempsRestant();
  //if (alarmeSetted())  // Si au moins 1 alarme a été définie
	afficheReveil();
  afficheBtMenu();
}
void afficheTempsRestant(void)
{
  lcd.setCursor(0,0); // Curseur en haut à gauche
  char message[16];
  char heures[3];
  char minutes[3];
  char secondes[3];
  
  clock heure;
  heure = clock_getClock();
  
  
  if (heure.secondes < 10)
  {
    sprintf(secondes, "0%d", heure.secondes);
  }
  else
  {
    sprintf(secondes, "%2d", heure.secondes);
  }
  
  if (heure.heures < 10)
  {
    sprintf(heures, "0%d", heure.heures);
  }
  else
  {
    sprintf(heures, "%2d", heure.heures);
  }
    
  if (heure.minutes < 10)
  {
    sprintf(minutes, "0%d", heure.minutes);
  }
  else
  {
    sprintf(minutes, "%2d", heure.minutes);
  }
  
  sprintf(message, "%s:%s:%s", heures, minutes, secondes);
  lcd.print(message);
}
void afficheBtMenu(void)
{
  lcd.setCursor(12,1);
  lcd.print("menu");
}


// MENU
Select_t afficheMenu(Menu_t myMenu)
{
  static uint8_t selection = 0;
  uint8_t retour = NO_SELECT;
  Select_t ret;
  
  if (popup != 1)
  {
    // Afficher le menu désiré
    lcd.setCursor(0,0);
    lcd.print(myMenu.titre);
    lcd.setCursor(0,1);
    lcd.print(myMenu.items[selection]);
    
    // Attent d'un événement
    if ( ! isEmpty_btfifo())
    {
      char event = get_btfifo();
      switch(event)
      {
        case BT_B_PRESSE:
          Serial.println("Bt bas");
          // choix suivant s'il existe
          if (selection < (myMenu.nbItem - 1))
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
			selection = myMenu.nbItem - 1;
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
  }
  return ret;
}
