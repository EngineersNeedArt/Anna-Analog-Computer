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
Needless to say, this makes the voltage window for representing values in ***Anna*** a very narrow 3 volts. Perhaps this is a good time to remind you that it is intended to be a *hobbyist* analog computer. 

## **MAIN**
The **MAIN** module is required for any ***Anna*** configuration as it provides USB power to the top two rails of the breadboard. These USB power rails are what all other ***Anna*** modules depend upon to operate.
- Provides USB power to the top two power rails on the breadboard—powering all other ***Anna*** modules.
- Provides the reference voltages for **+UNIT**, **ZERO** and **-UNIT**. These are the operating voltages of the analog computer programs.
- Inputs for up to five source voltages for display on the color LCD.
- Numerous metering modes (can be cycled through using the top button on the module).
- Overload LED (and output pin) indicate when a source voltage is out of range (higher than **+UNIT** or lower than **-UNIT**).

More (esoteric) details are available in the **README** that with the **MAIN** subfolder in this repository.

## **ADD**

The **ADD** module, as with the remaining modules described, is to be inserted into a breadboard such that its power pins engage in the top two power rails (top-most: **USB 5V**, and the next row: **GND**). As described above, the **USB** power is provided by the **MAIN** board.

As with most of the other modules described as well, it also requires a **ZERO** reference supplied to it from **MAIN**. Most of the modules use op-amps employing a *virtual ground* that is above **USB GND**. As indicated earlier, rather than the analog voltages swinging positive and negative to indicate positive and negative values, zero for ***Anna*** is 2.50V and positive and negative values run from 4.00V down to 1.00V. So that everyone is on the same page, the modules that require a virtual ground must have that connected to them from the **MAIN** module (a pin labeled **ZERO**).

The **ADD** module had a pair of addition operators (or *summers*—I don't know a *summer* is a season and an *adder* is a snake). These operators take 2 or more inputs (values, voltages) and output the negative sum of those inputs.

### Overload 

Analog computers have to be careful to avoid a condition referred to as **overloading**. The addition operation makes a good case for the problem. Given the voltage limitation of ***Anna*** (1V, *machine -1* or **-UNIT** in ***Anna** parlance to 4V, *machine +1* or **+UNIT**) what do we expect if we try to add 4V and 4V (or "max plus max" if you will)? To get technical, the underlying op-amps that comprise the analog circuitry cannot exceed their own power rails—which we have mentioned are **5V USB** and **GND**.

Instead, this condition is flagged as an error, called **overload**. The "programmer" must re-evaluate the program they have patched together—re-implement it to avoid overload. This might involve artificially scaling down part of an equation and then later "de-scaling" to counter.
