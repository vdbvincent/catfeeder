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

QueueArray<char> f_boutons;
QueueArray<char> f_moteur;
QueueArray<char> f_lcd;

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

#endif /* _CTRLEUR_H_ */
