# “Anna” Analog Computer

***Anna*** is an ecosystem of analog computing modules. The modules (each about the size of a matchbook) make use of a hobbyist breadboard and USB power that combine to make something like an analog computer trainer.

<p align="center">
<img src="images/ANNA Parts.jpeg" width="600">
</p>

### Breadboard

An electronics breadboard is an integral part of ***Anna***. The breadboard allows ***Anna*** to be easily modular, expandable. It provides a power bus (the top two power rails of the breadboard) and it is acts as the “patch bay” that allows you to connect inputs and outputs between the various analog operators.

### Power

The modules comprising ***Anna*** all run on (nominally 5V) USB power. All modules but **MAIN** assume that the top two power rails of the breadboard supply this USB power. The top rail (row) of the breadboard is assumed to be +5V and the rail (row) below is assumed to be ground (GND).

While these power rails operate the electronics for each module, these rails are not to be used for the analog *programming*. There are instead three reference voltages used in the analog computing domain. These are:
- **+UNIT**, nominally 4.00V. Representing the largest machine value (+1 if you like).
- **ZERO**, nominally 2.50V. Representing the machine value zero (0).
- **-UNIT**, nominally 1.00V. Representing the smallest machine value (-1 if you like).
Needless to say, this makes the voltage window for representing values in ***Anna*** a very narrow 3 volts. Perhaps this is a good time to remind you that it is intended to be a *hobbyist* analog computer. 

## **The Modules**

### MAIN Module

<p align="center">
<img src="images/MAIN Photo.jpeg" width="600">
</p>

The **MAIN** module is required for any ***Anna*** configuration as it provides USB power to the top two rails of the breadboard. These USB power rails are what all other ***Anna*** modules depend upon to operate.
- Provides USB power to the top two power rails on the breadboard—powering all other ***Anna*** modules.
- Provides the reference voltages for **+UNIT**, **ZERO** and **-UNIT**. These are the operating voltages of the analog computer programs.
- Inputs for up to five source voltages for display on the color LCD.
- Numerous metering modes (can be cycled through using the top button on the module).
- Overload LED (and output pin) indicate when a source voltage is out of range (higher than **+UNIT** or lower than **-UNIT**).

More (esoteric) details are available in the **README** that with the **MAIN** subfolder in this repository.

### POTS Module

<p align="center">
<img src="images/POTS Photo.jpeg" width="400">
</p>

The **POTS** module, as with the remaining modules described, is to be inserted into a breadboard such that its power pins engage in the top two power rails (top-most row: **USB 5V**, the row just below: **GND**). As described above, the **USB** power is provided by the **MAIN** board.

The **POTS** has four potentiometers—three of those are wired internally to behave as “coefficient multipliers” (in analog computer parlance). What this means, electrically, is that the low-side of the potentiometer is tied to a *machine zero* reference supplies by **MAIN**, while the user is expected to connect their input signal to the pin tied to the potʼs high-side. With the potʼs wiper conencted to the output pin, the pot acts as a voltage divider—effectivly attenuating the incoming voltage.

A practical use case for a coefficent multiplier is to attenuate a signal that is being used as feedback.

The 4th potentiometer on the **POTS** module is not tied to a zero-reference—the high-side of the pot is another input pin. The input pins are labeled **CCW** and **CW** referring to the rotation of the pot they correspond to. Connecting **CCW** to a **ZERO** reference makes this 4th pot behave like the other three—like a coefficient multiplier.

Like all of the modules (except for **MAIN**), **POTS** requires a **ZERO** voltage reference for the three coefficient multipliers to work correctly. The modules use op-amps employing a *virtual ground*. As indicated earlier, rather than the analog *voltages* swinging positive and negative to indicate positive and negative *values*, “zero” for ***Anna*** is 2.50V— positive values are from 2.50V to 4.00V, negative values from 2.50V down to 1.00V. So that all the modules are on the same page, they require the virtual ground connected to them from the **MAIN** module (a pin labeled **ZERO**).

(Strictly speaking, if you are using only the fourth potentiometer, you can leave off the **ZERO** reference input.)

All four pots buffer their output (using op-amps as voltage followers).

### ADD Module

<p align="center">
<img src="images/ADD Photo.jpeg" width="400">
</p>

The **ADD** module also requires **USB** power and also a **ZERO** reference voltage.

The **ADD** module has a pair of addition operators. (Or *summers*? I don't know: a “summer” is a season and an “adder” is a snake—they're both poor words). These operators take 2 or more inputs (values/voltages) and output the negative sum of those inputs.

#### Overload 

Analog computers programs have to be careful to avoid a condition referred to as **overloading**. The addition operation makes a good case for the problem. Given the voltage limitation of ***Anna*** (1V to 4V, i.e. *machine -1* to *machine +1*) what do we expect if we try to add 4V and 4V (*machine +1* plus *machine +1*)? To dive into the technical, the underlying op-amps that comprise the analog circuitry physically cannot exceed their own power rails—which we have mentioned are **5V USB** and **GND**. So a sum of 8V is already out of the question.

Instead, this condition is considered an error, called **overload**. The "programmer" must re-evaluate the program they have patched together—re-implement it to avoid overload. This might involve artificially scaling down part of an equation and then later "de-scaling" to counter.

There is no on-board overload warning on the **ADD** module. Wiring the ouput of any add operator to one of the voltage source inputs of **MAIN** though will catch any overloads (**MAIN** detects over/undervolts at all its inputs).

Also, going too negative (below 1V in the case of ***Anna***) is oddly also called an *overload*.

#### Inversion

You may notice that the add operators return the *inverted* sum of the inputs. That is, if you input the values for 0.2 and 0.4, the sum operator will return not 0.6 but -0.6. There is an underlying (op-amp related) reason for this. When the result of adding is needed to act as negative feedback in an equation you are modelling, this inversion is a happy accident. At times though when you need the non-inverted sum, you will have to wire the output of a summer to an inverter operator.

Any add operator with a single input will return that input value inverted as output. But since inverting is a common operation, the **ADD** module contains a pair of dedicated inverter operators. (A dedicated inverter of course needs less pins and so conserves breadboard real-estate).

### MULT Module

<p align="center">
<img src="images/MULT Photo.jpeg" width="400">
</p>

The **MULT** module has a pair of multipliers. It performs multiplication using a PWM circuit (described in more detail in the **MULT** ReadMe.md). It requires **USB** power and a **ZERO** reference voltage.

The multiplier and multiplicand are assumed to be within *machine range* and are treated as values between 1.0 and -1.0. As such, the product will also always be within *machine range* (between 1.0 and -1.0). Consequently, the operator should not overload. (If the product ever overloads it is from an input that was already overloaded.)

A test point and two trim pots are there to allow the mutliplier to be calibrated. (This requires an oscilloscope in order to adjust a triangle wave to run between 1.00 and 4.00 Volts.) After calibrating a **MULT** module, I measured it with 49 sample voltages (combinations of 7 voltages for **X** and 7 for **Y**) and measured the product. Plotting the results vs. the expected result I submitted the data to an LLM and was told the overall accuracy to be about 2.3% RMS full scale (I am lazy beyond some threshold). In general, expect some error from the **MULT** module.

### INT Module

<p align="center">
<img src="images/INT Photo.jpeg" width="400">
</p>

The **INT** module may be the most important one. I have read that integration was in fact the reason for early analog computing at all.

**INT** combines a pair of integrators in a single module. A single **Reset** button, when held for a short period, sets both integrators to their initial conditions (initial voltages). The “IC” of each integrator is established from the user having connected a voltage to its **IC** pin.

Integrators output a voltage—initially they are expected to be Reset and so ouput their IC voltage. Inputs to each integrator though can cause the output voltage of the integrator to rise or fall—the applied voltage or value is being integrated with respect to time.

Like the **ADD** module, the integrators on the **INT** module are also inverting. In practice this means that if you supply a **positive** *machine value* to an integrator, the value it ouyputs will get **smaller**, not larger. The reverse is the case when a **negative** machine value is applied.

If *machine zero* (**ZERO**) is applied as in input, the output of an integrator does not change.

Integrators integrate with respect to time and there is a switch on **INT** that can toggle between a *slow* integration time constant and a *fast* one. In practice, most simulations (programs), might prefer the faster integration—such as a program that models the bounce of a mass on a spring. Another program, like a game simulating a spacecraft landing on the Moon, might want to be slowed down in order to be more easy to play.

## **Future Modules**

There are some “missing modules” that will perhaps show up over time.

### Log and Anti-log Modules

Searching through older analog computer sources, you can find circuits that take a voltage as input and then output the natural **log** of that voltage. A diode (or more typically, a transistor) is employed in the *feedback loop* of an op-amp to achieve this.

Further, a diode (transistor) employed in *series* with the input to an op-amp will give you the **anti-log** from an input voltage.

*(It might be noted that the accuracy of both circuits is (apparently) encumbered by their sensitivity to temperature.)*

If a value is first passed to the **log** circuit, the result can be divided by two with a **coefficient multiplier**—the **anti-log** of this then gives you the square root of the original value.

Or, the **log** of two values can be added with an **adder** and the **anti-log** of the sum is the equivalent of the product of the original values. (Another method of multiplication.)

### Comparator Module

For some simulations a kind of *binary switch* might be desirable. For example, a bouncing ball simulation needs to know when the ball has fallen to some position where it has encountered the floor and should reverse its velocity (it should bounce). A **comparator** module would make use of an op-amp to detect when the input has crossed some threshhold and then switch its output.

### Sweep Module

Some simulations might benefit by having an input that sweeps a range of values. A circuit that outputs a triangle wave would achive this. Having a means to change the speed (period) of the sweep would be important.

### Trigonometry Module

Sine and cosine are uiseful operations to include. There are no simple electrical circuits that naturally give rise to these trigonometric functions. There is a method using diodes and resistors to *shape* a function from a given input. Enough diode stages in a kind of ladder configuration with precise resistor values can output a voltage that approximate, for example, the sine of the input voltage.
