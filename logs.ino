/*
* logs.c
*
* Created on: 31 juil. 2015
* Author: vincent
*/
#include "logs.h"

#define FILTRE  3 // Filtrage de l'affichage des logs. 0 = Rien afficher
//													   1 - Afficher les logs ERREUR
//													   2 - Afficher les logs ERREUR + INFO
//													   3 - Afficher les logs ERREUR + INFO + DEBUG

static Bool logs_isInit = False;

void logs_setup(void)
{
	Serial.begin(9600);
	Serial.println("Init du logs");
	logs_isInit = True;
}

void logs_every10ms(void)
{
	// regarder si on a qlq chose dans la fifo de log
	// puis l'envoyer sur la ls
	Logs_t objLog;
	char buffer[2048];
	char * crit;
	
	if ((! isEmpty_logfifo()) && (logs_isInit == True))
	{
		objLog = get_logfifo();

		// Filtrage
		if (objLog.crit <= FILTRE)
		{
			if (objLog.crit == ERREUR)
				crit = "ERREUR";
			else if (objLog.crit == INFO)
				crit = "INFO";
			else if (objLog.crit == DEBUG)
				crit = "DEBUG";

			// Affichage sur LS
			sprintf(buffer, "[%] - [%s]", crit, objLog.texte);
			
			Serial.println(buffer);
		}
	}
}
