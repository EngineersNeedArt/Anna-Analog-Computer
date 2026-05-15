# *〝Anna〞* (a breadboard-based, modular, hobbyist analog computer)

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

## **POTS**

The **POTS** module, as with the remaining modules described, is to be inserted into a breadboard such that its power pins engage in the top two power rails (top-most row: **USB 5V**, the row just below: **GND**). As described above, the **USB** power is provided by the **MAIN** board.

The **POTS** has four potentiometers—three of those are wired internally to behave as “coefficient multipliers” (in analog computer parlance). What this means, electrically, is that the low-side of the potentiometer is tied to a *machine zero* reference supplies by **MAIN**, while the user is expected to connect their input signal to the pin tied to the potʼs high-side. With the potʼs wiper conencted to the output pin, the pot acts as a voltage divider—effectivly attenuating the incoming voltage.

A practical use case for a coefficent multiplier is to attenuate a signal that is being used as feedback.

The 4th potentiometer on the **POTS** module is not tied to a zero-reference—the high-side of the pot is another input pin. The input pins are labeled **CCW** and **CW** referring to the rotation of the pot they correspond to. Connecting **CCW** to a **ZERO** reference makes this 4th pot behave like the other three—like a coefficient multiplier.

Like all of the modules (except for **MAIN**), **POTS** requires a **ZERO** voltage reference for the three coefficient multipliers to work correctly. The modules use op-amps employing a *virtual ground*. As indicated earlier, rather than the analog *voltages* swinging positive and negative to indicate positive and negative *values*, “zero” for ***Anna*** is 2.50V— positive values are from 2.50V to 4.00V, negative values from 2.50V down to 1.00V. So that all the modules are on the same page, they require the virtual ground connected to them from the **MAIN** module (a pin labeled **ZERO**).

(Strictly speaking, if you are using only the fourth potentiometer, you can leave off the **ZERO** reference input.)

All four pots buffer their output (using op-amps as voltage followers).

## **ADD**

The **ADD** module also requires **USB** power and also a **ZERO** reference voltage.

The **ADD** module has a pair of addition operators. (Or *summers*? I don't know: a “summer” is a season and an “adder” is a snake—they're both poor words). These operators take 2 or more inputs (values/voltages) and output the negative sum of those inputs.

### Overload 

Analog computers have to be careful to avoid a condition referred to as **overloading**. The addition operation makes a good case for the problem. Given the voltage limitation of ***Anna*** (1V, *machine -1* or **-UNIT** in ***Anna** parlance to 4V, *machine +1* or **+UNIT**) what do we expect if we try to add 4V and 4V (or "max plus max" if you will)? To get technical, the underlying op-amps that comprise the analog circuitry cannot exceed their own power rails—which we have mentioned are **5V USB** and **GND**.

Instead, this condition is flagged as an error, called **overload**. The "programmer" must re-evaluate the program they have patched together—re-implement it to avoid overload. This might involve artificially scaling down part of an equation and then later "de-scaling" to counter.

There is no on-board overload warning on the **ADD** module. Wiring the ouput of any add operator to one of the voltage source inputs of **MAIN** though will catch any overloads (**MAIN** detects over/undervolts at all its inputs).

Also, going too negative (below 1V in the case of ***Anna***) is oddly also called an *overload*.

### Inversion

You may notice that the add operators return the *inverted* sum of the inputs. That is, if you input the values for 0.2 and 0.4, the sum operator will return not 0.6 but -0.6. There is an underlying (op-amp related) reason for this. When the result of adding is needed to act as negative feedback in an equation you are modelling, this inversion is a happy accident. At times though when you need the non-inverted sum, you will have to wire the output of a summer to an inverter operator.

Any add operator with a single input will return that input value inverted as output. But since inverting is a common operation, the **ADD** module contains a pair of dedicated inverter operators. (A dedicated inverter of course needs less pins and so conserves breadboard real-estate).

## **MULT**

## **INT**
