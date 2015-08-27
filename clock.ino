/*
 * clock.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "clock.h"


// Définition des variables globales
char heures;
char minutes;
char secondes;
int millisecondes;

void clock_setup(void)
{
  //Serial.begin(9600);
  
  clock_reset();
}

void clock_every1ms(void)
{
  /* Variables locales de gestion du temps */
  static unsigned long last_time = 0; // Temps antérieur
  unsigned long time_now = millis();  // Temps actuel

  // Et que 1 sec s'est écoulée
  if(time_now - last_time >= 1000)
  {
    secondes ++;
    last_time = time_now;
  }


  //  incrément des secondes
  if (secondes >= 60)
  {
    secondes = 0;
    minutes ++;
  }
  // incrément des minutes
  if (minutes >= 60)
  {
    minutes = 0;
    heures ++;
  }
  if (heures >= 24)
  {
    heures = 0;
    minutes = 0;
    secondes = 0;
    millisecondes = 0;
  }

}
void clock_every10ms(void)
{
  // incrément des ms
  if (millisecondes >= 100)
  {
    millisecondes = 0;
    secondes ++;
  }
  
  //  incrément des secondes
  if (secondes >= 60)
  {
    secondes = 0;
    minutes ++;
  }
  
  // incrément des minutes
  if (minutes >= 60)
  {
    minutes = 0;
    heures ++;
  }
  
  if (heures >= 24)
  {
    heures = 0;
    minutes = 0;
    secondes = 0;
    millisecondes = 0;
  }
  
  millisecondes ++;
}


// GETTER  //  SETTER
clock clock_getClock(void)
{
  clock myclock;
  myclock.heures = heures;
  myclock.minutes = minutes;
  myclock.secondes = secondes;
  return myclock;
}

void clock_setClock(clock p_myclock)
{
  heures = p_myclock.heures;
  minutes = p_myclock.minutes;
  secondes = p_myclock.secondes;
  
  char message[16];
  sprintf(message, "%d:%d:%d", p_myclock.heures, p_myclock.minutes, p_myclock.secondes);
  Serial.println(message);
}

void clock_reset(void)
{
  heures = 0;
  minutes = 0;
  secondes = 1;
  millisecondes = 0;
}