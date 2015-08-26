/*
 * ctrleur.ino
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#include "QueueArray.h"
#include "ctrleur.h"




// GESTION DES FIFOS
void put_btfifo(char event)
{
  f_boutons.enqueue(event);
}

Bool isEmpty_btfifo(void)
{
  return (Bool)f_boutons.isEmpty();
}

char get_btfifo(void)
{
  return f_boutons.dequeue();
}

Bool isFull_btfifo(void)
{
  return (Bool)f_boutons.isFull();
}

void put_mtfifo(char event)
{
  f_moteur.enqueue(event);
}

Bool isEmpty_mtfifo(void)
{
  return (Bool)f_moteur.isEmpty();
}

char get_mtfifo(void)
{
  return f_moteur.dequeue();
}

Bool isFull_mtfifo(void)
{
  return (Bool)f_moteur.isFull();
}

// FIFO LCD
void put_lcdfifo(char event)
{
  f_lcd.enqueue(event);
}

Bool isEmpty_lcdfifo(void)
{
  return (Bool)f_lcd.isEmpty();
}

char get_lcdfifo(void)
{
  return f_lcd.dequeue();
}

Bool isFull_lcdfifo(void)
{
  return (Bool)f_lcd.isFull();
}
