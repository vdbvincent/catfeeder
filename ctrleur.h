/*
 * ctrleur.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#ifndef _CTRLEUR_H_
#define _CTRLEUR_H_

// Libs
#include "QueueArray.h"
#include "common.h"
#include "clock.h"

QueueArray<char>   f_boutons;
QueueArray<char>   f_moteur;
QueueArray<char>   f_lcd;
QueueArray<Logs_t> f_log;

// FIFO boutons
void put_btfifo(char event);
Bool isEmpty_btfifo(void);
char get_btfifo(void);
Bool isFull_btfifo(void);

// FIFO Moteur
void put_mtfifo(char event);
Bool isEmpty_mtfifo(void);
char get_mtfifo(void);
Bool isFull_mtfifo(void);

// FIFO LCD
void put_lcdfifo(char event);
Bool isEmpty_lcdfifo(void);
char get_lcdfifo(void);
Bool isFull_lcdfifo(void);

// FIFO Logs
void put_logfifo(Logs_t obj);
Bool isEmpty_logfifo(void);
Logs_t get_logfifo(void);
Bool isFull_logfifo(void);
void print_log(Criticite_t crit, char * texte);

#endif /* _CTRLEUR_H_ */
