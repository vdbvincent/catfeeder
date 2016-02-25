/*
* logs.c
*
* Created on: 31 juil. 2015
* Author: vincent
*/
#include "logs.h"
#include "QueueArray.h"


// Définition des variables globales
//QueueArray<Logs_t> f_log;  // Fifo de logs
QueueArray<Logs_t> f_log;  // Fifo de logs

void logs_setup(void)
{
}

void logs_idle(void)
{
	clock mycl;
	// regarder si on a qlq chose dans la fifo de log
	// puis l'envoyer sur la ls
	int i = 0;
	Logs_t objLog;
	char buffer[MAX_SIZE_LOG+11];

	if (! f_log.isEmpty())
	{
		sprintf(buffer, "");
		objLog = f_log.pop();

		mycl = clock_getClock();
		char message[10];
	    sprintf(message, "[%02d:%02d:%02d]", mycl.heures, mycl.minutes, mycl.secondes);
		
		// Filtrage
		if (objLog.crit <= FILTRE)
		{
			switch (objLog.crit)
			{
			    case ERREUR:
					sprintf(buffer, "[ERREUR] - %s - %s\n", message, objLog.texte);
			      	break;
			    case INFO:
					sprintf(buffer, "[ INFO ] - %s - %s\n", message, objLog.texte);
			      	break;
			    case DEBUG:
					sprintf(buffer, "[ DEBUG] - %s - %s\n", message, objLog.texte);
			    	break;
			    default:
			    	sprintf(buffer, "%s\n", objLog.texte);
			      	break;
			}
			printstr(buffer);
		}
	}
}


void print_log(char crit, char * texte)
{
	uint8_t i = 0;
	Logs_t mylog;
	mylog.crit = crit;

	// Avec cette boucle, on calcule la taille du char * et on rejete si hors limite
	// On ne prend pas non plus le dernier caractere \n car on reformate le log dans idle
	while((texte[i] != '\n') && (i < MAX_SIZE_LOG-1))
	{
		mylog.texte[i] = texte[i];
		i++;
	}
	if (i >= MAX_SIZE_LOG-1)
	{
		printstr(F("logs : ERR MAX SIZE\n"));  // message tronqué
	}

	mylog.texte[i] = '\0';

	if (! f_log.isFull())
		f_log.push(mylog);
	else
		printstr(F("logs : ERR FULL FIFO\n"));
}
