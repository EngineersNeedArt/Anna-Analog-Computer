# MULT Module

The **MULT** module has two multiplier operators. Both take a pair of values assumed to be within *machine range* (between 1.0V and 4.0V). Each outputs the product of the two inputs. The outputs (products) from the **MULT** module are not inverted as they are for the **INT** and **ADD** module. This is because the **MULT** module works very differently (as is discussed below).

### ZERO

Before going further though, like the other operator modules, *machine zero* (**ZERO**, 2.5V) is a required input. The usual way to do this is to connect a wire from the **MAIN** module's **ZERO** voltage output.

### Multiplication Operation

As stated, the two values to be multiplied are assumed to be between *-UNIT* and *+UNIT*. The multiplier operators treat this range of input values as though they represent -1.0 to +1.0. With -1.0 (*-UNIT*) being the smallest value and +1.0 (*+UNIT*) the largest value to be multiplied, it should be clear that, mathematically, the product too will always be within this range. For this reason, the **MULT** module should not ever be a source of *overflow*. (The product should only overflow if one of the multiplicands was already overflowing.)

### Circuit Operation

As stated, multiplication is done in a way very different from the op-amp based modules. Describing how it works is simplified if it is broken down in steps.

<div align="center">
  <img src="images/MULT Schematic.png" alt="MULT Schematic" width="910">
</div>

### Triangle Wave

Top-most in the schematic (above) is a circuit that outputs a triangle wave. It consists of a pair of op-amps: the first one acting as an oscillator to create an intermediary square wave, the second op-amp is wired as an integrator such that the incoming square wave causes the final triangular output.

Two trim pots are incorporated in order to adjust the triangle wave such that it peaks at 4.0V (*+UNIT*) and at it's lowest, base voltage, it is at 1.0V (*-UNIT).

The purpose of the triangle wave (that oscillates at a freuqncy of about 4 kHz, FWIW) will be clear below.

### Comparators

Just below the triangle wave circuit (see schematic above again) is one of the multiplicand inputs labeled **X1**. It is connected to the non-inverting input of an op-amp (*U1B*). The inverting input of that op-amp if the triangle wave we described. With op-amp configured as a comparator, the output is going to be high when the non-inverting input is a greater voltage than the inverting input. For our purposes, it means that the op-amp output is high when the multipilcand (*X1*) is greater than the value of the triangle wave.

Let's stop and think about what that means. If **X1** is *machine zero* (recall, 2.5V) and the triangle wave is ramping up and down symmetrically between 1.0V (*-UNIT*) and 4.0V (*+UNIT*), then it should be the case that the output from the comparator is split 50/50 between being high and being low.

If instead **X1** is greater than *machine zero* (greater than 2.5V) then the output from **U1B** should be higher more than 50% of the time. If this sounds to you like **X1** is controlling the *duty cycle* of comparator **U1B**, pat yourself on the back.

You should note that an identical circuit to the one just described exists below on the schematic—for **X1** and **U2B**. I will only be describing the one circuit.

### Switching

