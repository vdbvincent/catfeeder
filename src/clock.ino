/*
 * clock.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "clock.h"
#include <Wire.h>
#include <RTClib.h>

// Fréquence d'appel de la méthode every100ms()
#define FREQ_SEQ 100

// Définition des variables globales
uint8_t heures = 0;
uint8_t minutes = 0;
uint8_t secondes = 0;
uint16_t millisecondes = 0;
uint8_t mb_clockInit = 0;  // Permet de garder en mémoire la mise à l"heure
uint8_t g_Ctmp = 0;

RTC_DS1307 myRTC;

void clock_setup(void)
{
      clock_reset();
      Wire.begin();
      myRTC.begin();

      // following line sets the RTC to the date & time this sketch was compiled
      //myRTC.adjust(DateTime(__DATE__, __TIME__));
      procCtmp();
}

void clock_every100ms(void)
{
      static uint8_t compteur_reset = 0;
      
      // incrément des ms
      if (millisecondes >= 1000)
      {
            millisecondes = 0;
            secondes ++;
      }

      // Mettre a jour l'heure toutes les minutes
      // avec l'heure atomoique
      if (secondes >= 0)
            procCtmp();

      //  incrément des secondes
      if (secondes >= 60)
      {
            secondes = 0;
            minutes ++;
            #ifdef CDEBUG
            print_log(DEBUG, "tic minute\n");
            #endif
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

      millisecondes += FREQ_SEQ;
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
      mb_clockInit = 1; // Permet de garder en mémoire la mise à l"heure

      clock_adjust(p_myclock);
      
      #ifdef CDEBUG
      char message[18];
      sprintf(message, "clock set: %02d:%02d\n", heures, minutes);
      print_log(DEBUG, message);
      #endif
}

void clock_reset(void)
{
      heures = 0;
      minutes = 0;
      secondes = 1;
      millisecondes = 0;
}

uint8_t clock_isInit(void)
{
      return mb_clockInit;
}

// fonction permettant de lire l'heure courante sur le shiel RTC
void clock_maj(void)
{
      DateTime now = myRTC.now();

      heures = now.hour();
      minutes = now.minute();
      secondes = now.second();
      millisecondes = 0;
}


// fonction permettant de mettre a jour l'heure régulierement
void clock_idle(void)
{
      if (g_Ctmp == 1)
      {
            g_Ctmp = 0;
            clock_maj();
      }
}
void procCtmp(void)
{
      g_Ctmp = 1;
}

void clock_adjust(clock p_myclock)
{
      DateTime now(2017, 5, 22, p_myclock.heures, p_myclock.minutes, p_myclock.secondes);
      myRTC.adjust(now);
}