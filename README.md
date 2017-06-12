# catfeeder

Example for building your own automatic catfeeder built with Arduino uno


## About

This is a example, with software provided, on how automate feeding your pets.

This repo contains schemas, and source code.

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

== VIDEO GIF ==
![feed_gif]()

## Menu interface

== photos des écrans avec flow ==

## Electronics

### Components list
- Arduino Uno
- 9v DC/DC
- Relay
- Power supply

### Diagram
![alt_text](https://github.com/vdbvincent/catfeeder/blob/master/schema/shema_catfeeder.jpg)

### Frame
AJouter quelques photos du chassis

## Contribute
Please, feel free to send me a pull request

## Next
* Add RF communication
* Add android interface for settings
* Add a feelling sensor

