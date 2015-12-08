/*
 * main.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
// Inclusions
//#include "mcubind.h"
//#include <MsTimer2.h> // inclusion de la librairie Timer2
//#include "boutons.h"
//#include "lcd.h"
//#include "clock.h"
//#include "moteur.h"
//#include "menu.h"
//#include "alarme.h"
/*
// Variables globales
static int c10 = 0 ;

void setup()
{
	// init du process bouton avec un delai de filtrage de 2*4=8ms
	boutons_setup(2);
	// Init de l'écran LCD
	lcd_setup();
	// Init de l'horloge
	clock_setup();
	// Init du moteur
	moteur_setup();
	// Init du memu
	menu_setup(300);  // 2sec de tempo pour l'écran de démarrage
	// Init de l'alarme
	alarme_setup();

	// initialisation interruption Timer 2
	//MsTimer2::set(1, InterruptTimer2); // période 10ms 
	//MsTimer2::start(); // active Timer2 
}

// debut de la fonction d'interruption Timer2
void InterruptTimer2()
{
	// TOUTES LES 10 MILLISECONDES
	switch (c10) 
	{
		case 0:
			boutons_every10ms();
		break;
		case 1:
			lcd_every10ms();
		break;
		case 2:
			clock_every10ms();
		break;
		case 3:
			menu_every10ms();
		break;
		case 4:
			alarme_every10ms();
		break;
		case 5:
		// do something
		break;
		case 6:
		// do something
		break;
		case 7:
		// do something
		break;
		case 8:
		// do something
		break;
		case 9:
		// do something
		break;
	}
	if (c10 >= 10)
		c10 = 0;
	else
		c10 ++;

}
*/
int value = 2;
char buffer[20];
void setup()
{
	Serial.begin(115200);
	Serial.println("coucou");
}

void loop()
{
	// envoi du message coucou sur le port série
	//printstr("hello world\n");
	value = analogRead(0);
	sprintf(buffer, "valeur = %d v\n", value * 5 / 1023);
	//printstr(buffer);
	Serial.println(buffer);
	delay(1000);

	// Idle
	//clock_every1ms();  // trop rapide
	//moteur_every1ms();
}
/*


int main(void)
{
	// init
	//init_mcubind();
	// init du process bouton avec un delai de filtrage de 2*4=8ms
	//boutons_setup(1);
	//printstr("init ok\n");
	
	Serial.begin(115200);
	//pinMode(0, INPUT);
	// init d'u port 5 en sortie
	//mcubind_virtualport_init(MCUBIND_VIRTUALPORT_B_b5, 1);
	//mcubind_virtualport_write(MCUBIND_VIRTUALPORT_B_b5, 0);

	Serial.println("coucou");
	// iddle
	while(1)
	{
	
		// envoi du message coucou sur le port série
		//printstr("hello world\n");
		value = analogRead(0);
		sprintf(buffer, "valeur = %d v\n", value * 5 / 1023);
		//printstr(buffer);
		Serial.println(buffer);
		delay(1000);
	}

	return 0;
}

/*
void every500ms(void)
{
	static uint8_t flag = 1;

	if (flag)
	{
		
		flag = 0;
	}
	else
	{
		flag = 1;
	}
	mcubind_virtualport_write(MCUBIND_VIRTUALPORT_B_b5, flag);

	//boutons_every10ms();
	
	
}*/