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

Two trim pots are incorporated in order to adjust the triangle wave such that it peaks at 4.0V (*+UNIT*) and at it's lowest, base voltage, it is at 1.0V (-UNIT).

The purpose of the triangle wave (that oscillates at a frequency of about 4 kHz, FWIW) will be clear below.

### Comparators

Just below the triangle wave circuit (see schematic above again) is one of the multiplicand inputs labeled **X1**. It is connected to the non-inverting input of an op-amp (*U1B*). The inverting input of that op-amp if the triangle wave we described. With op-amp configured as a comparator, the output is going to be high when the non-inverting input is a greater voltage than the inverting input. For our purposes, it means that the op-amp output is high when the multiplicand (*X1*) is greater than the value of the triangle wave.

Let's stop and think about what that means. If **X1** is *machine zero* (recall, 2.5V) and the triangle wave is ramping up and down symmetrically between 1.0V (*-UNIT*) and 4.0V (*+UNIT*), then it should be the case that the output from the comparator is split 50/50 between being high and being low.

If instead **X1** is greater than *machine zero* (greater than 2.5V) then the output from **U1B** should be higher more than 50% of the time. If this sounds to you like **X1** is controlling the *duty cycle* of comparator **U1B**, pat yourself on the back.

You should note that an identical circuit to the one just described exists below on the schematic—for **X1** and **U2B**. I will only be describing the one circuit.

### Switching

Following the schematic, the pulse-width modulated (PWM) output from **U1B** goes to the rather large rectangular circuit/chip. This integrated circuit is an analog switch, and **U1B** is connected to drive one of the internal switches (*S1*). This is the "A" switch in the IC and it controls whether **A0** or **A1** are selected for output **A**.

Recall that if **X1** is *machine zero* we expect a 50% duty-cycle. This would mean that **A** would output **A0** half of the time and **A1** the other half of the time. And what then are **A0** and **A1**?

**A1** is simple, it comes directly from **Y1**, our multiplier.

**A0** is a little more complicated, it comes from an op-amp (*U2A*) acting as an inverting op-amp. In fact, **A0** is **Y1** inverted (the negative value). So if **Y1** is *+UNIT*, **U2A** will output *-UNIT*. Another example, if **Y1** is *machine zero*, the inverted value is also *machine zero*.

What does this mean if we are switching between **Y1** and inverted **Y1**? Before we answer that question, let's look at the final stage of the circuit.

### Filtering

The final stage for our multiplier operator that began with is **X1** and **Y1** is the op-amp (*U1C*) that outputs the product (**X1×1**). This final op-amp is configured as an active filter. It *smooths* the output of switch **A** to a voltage (value) that is the kind of average of the output.

Back to our early example, if we are switching between **Y1** and **-Y1** (inverted **Y1**) and spending 50% of the time at each voltage, the *average* value will be *machine zero*. Does that make sense? If **Y1** is *+UNIT* then **-Y1** will be *-UNIT* and, when averaged, will give *machine zero*.

In fact, this is what we expect mathematically: recall that 50% duty cycle was for an **X1** that was *machine zero*—and any number multiplied by zero if of course zero.

So what happens when **X1** is greater than *machine zero*? We said above that the output of our comparator would be high more often than low in that case. This will correspond to **Y1** being switched in more often than the inverted **-Y1**. And this will *average* out to a value closer to **Y1**.

In the extreme case, if **X1** is *+UNIT* (the largest value and treated as though it is numerically 1.0) then the output of the comparator should remain high and the switched output then simply **Y1**. Again, this is expected as 1.0 times any number is that number.

I'll leave it as an exercise for you to understand the operation of the circuit when **X1** is *-UNIT* or when **Y1** is *machine zero*.

## TODO

• An inverted output would be nice: **-X × Y**. This would require additional op-amps.

• There is a cap that is a touch too close to the edge of the PCB.

• Silkscreen show **X × Y** instead of **XY**. (Perhaps **X ⋅ Y**)

• Larger butterfly art.

• Possibly replace trim pots with specific resistor values found to be correct.
