# catfeeder

Example for building your own automatic catfeeder built with Arduino uno


## About

This is a example, with software provided, on how automate feeding your pets
This repo contains schemas, and code source.

## How does it work ?

The feeder dispenser is very simple.
It consists of a self-made wood frame and an archimede screw powered by an arduino uno.
The arduino drives the motor which drives the screw. The food is then pushed into a spilway.
It offers a menu, througn a LCD screen, in which find :
- Time programming : you need to enter the time at fisrt power-up.
- Alarms programming : up to 5 alarms can be programmed in a day to program a food sitribution.
- Food distribution : you can choose the amount of food to be dispensed and then operate the engine.

A sleep mode id used to turn off the LCD scren to prevent overcurrent. It appear after 30 secondes of inactivity.
In this mode, the 'OK' button switches the display back on. The others help to distribute food quickly.
