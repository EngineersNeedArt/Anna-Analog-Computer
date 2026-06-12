# INT Module

The **INT** module consists of a pair of integrators. One of the integrators has three inputs and the other has two.

<div align="center">
  <img src="images/INT Schematic.png" alt="INT Schematic" width="770">
</div>

## Zero

It will probably come as no surprise that the **INT** module also expects a *machine zero* reference as input. Like the **ADD** module, *machine zero* (ZERO) is used as a *virtual ground* on the non-inverting input of the op-amps.

Like the **ADD** module, this is why the output of the integrator is inverted. Since inout values/voltages are connected to the inverting input, they have an inverse effect on the output of the integrator. A positive value (voltage above 2.5V) will in fact cause the integrator output to decrease while a negative value (a voltage below 2.5V) causes the integrator output to become more positive.

## Reset

Each integrator has an *initial condition* (IC) input. This is a value/voltage that the user supplies to be the starting value (output) for each integrator. A single momentary switch, when held down for a short time, resets both integrators to this initial condition.

## Integration Speed

Unlike the other **“Anna”** modules, the integrators have a kind of *speed* associated with them—the speed of integration. The value of the input resistors contribute to determine the speed of integration as does the size of the capacitor in the op-amp feedback circuits.

It was found that some simulations, like *Lunar Lander* for example, preferred a slower integration time—to allow the player to react to their descending spacecraft. Other simulations, such as a mass on a spring, felt more realistic with a faster integrator.

To address either scenario, the **INT** module has a separate switch for each integrator that allows for toggling in or out of the op-amp feedback an additional capacitor. With just the one capacitor switched in, an integrator will run faster for simulations like the *spring and mass*. Switching in a second capacitor slows the integration down by roughly an order of magnitude—making a game like *Lunar Lander* playable.

## Summing Junctions

Like the **ADD** module, the **INT** module also makes accessible the “summing junction” for each integrator. The summing junction is where all the inputs tie together and connect to the inverting input of an op-amp.

If a user wishes for an additional input, a 1 MΩ input resistor can be connected in series with the input and one of the summing junctions on the **INT** module.

Alternatively, if a smaller resistor value is used as an input resistor, such as a 100 kΩ resistor, that input will move the integrator 10x (ten times) as fast. It is possible then to have a much quicker integrator (or much slower) by using input resistors other than 1 MΩ.
