# *〝Anna〞* (a modular, hobbyist-grade analog computer)

Anna is an ecosystem of hobbyist-grade, analog computing modules.

## Breadboard

An electronic’s breadboard is an integral part of ***Anna***. Photos of ***Anna*** on a breadboard should not be thought to be of ***Anna*** as a prototype. This *is* ***Anna***. The breadboard allows ***Anna*** to be easily modular, expandable. It provides a power bus (the top two power rails of the breadboard) and it is acts as the “patch bay” that allows you to connect inputs and outputs between the various analog operators.

## Power

The modules comprising ***Anna*** all run on (nominally 5V) USB power. All modules but **MAIN** assume that the top two power rails of the breadboard supply this USB power. The top rail (row) of the breadboard is assumed to be +5V and the rail (row) below is assumed to be ground (GND).

While these power rails operate the electronics for each module, these rails are not to be used for the analog *programming*. There are instead three reference voltages used in the analog computing domain. These are:
- **+UNIT**, nominally 4.00V. Representing the largest machine value (+1 if you like).
- **ZERO**, nominally 2.50V. Representing the machine value zero (0).
- **-UNIT**, nominally 1.00V. Representing the smallest machine value (-1 if you like).
Needless to say, this makes the voltage window for representing values in ***Anna*** a very narrow 3 volts. Perhaps this is a good time to remind you that it is intended to be a *hobbyist* ananlog computer. 

## **MAIN**
The **MAIN** module is required for any ***Anna*** configuration as it provides USB power to the top two rails of the breadboard. These USB power rails are what all other ***Anna*** modules depend upon to operate.
- Provides USB power to the top two power rails on the breadboard—powering all other ***Anna*** modules.
- Provides the reference voltages for **+UNIT**, **ZERO** and **-UNIT**. These are the operating voltages of the analog computer programs.
- Inputs for up to five source voltages for display on the color LCD.
- Numerous metering modes (can be cycled through using the top button on the module).
- Overload LED (and output pin) indicate when a source voltage is out of range (higher than **+UNIT** or lower than **-UNIT**).

Etc…

