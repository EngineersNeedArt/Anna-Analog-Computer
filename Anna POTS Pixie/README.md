# POTS Module

Four op-amps buffer the outputs from the four potentiometers of the **POTS** Module. In this way, circuits connected to the output to not load the potentiometer and drag down itʼs voltage.

<div align="center">
  <img src="images/POTS Schematic.png" alt="POTS Schematic" width="765">
</div>

## Coefficient Multipliers

Again, as with the other modules (excluding **MAIN**), we have a **ZERO** input that is expected to be the *Zero* machine value (2.5V provided by **MAIN**). In the case of **POTS** though it is used merely as the CCW (counter-clockwise) input on each of three potentiometers. In this way, the potentiometers behave as (to use analog computing terminology) *coefficient multipliers*. That is, given an input value/voltage on the other end of the pot, the wiper of the pot will present (output) a value/voltage between input and *machine zero*. In this way it is *attenuating* the input (multiplying by a factor between 0.0 and 1.0).

A common use-case for a coefficient multiplier in analog computing might be to feedback some portion of a value, perhaps from an integrator, back into itʼs input. For example, if an integrator is representing velocity, a portion of that velocity might be used to represent the friction impeding the velocity. A coefficient multiplier allows you to dial in a coefficient of friction.

Another common use case is to present a value/voltage less than the *positive machine unit* or greater than the *negative machine unit*.

## Open-Ended Potentiometer

The fourth potentiometer is not tied to *machine zero*—both ends of the potentiometer are user inputs. This is more open-ended potentiometer—allowing the user to connect any value/voltage pair to the inputs and use the pot knob to select an output value/voltage anywhere in between. If *positive machine unit* is connected as one input and *negative machine unit* connected to the other, the output (wiper) can represent all machine values for **“Anna”**.

Further, if you should connect *machine zero* to the CCW (counter-clockwise) input, you have a fourth coefficient multiplier.

## Power

Not shown in the schematic, the op-amps, again, are powered from the (nominal) 5V USB power in the same way the rest of **“Annaʼs”** modules are.
