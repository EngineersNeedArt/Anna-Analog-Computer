# ADD Module

Like the other **“Anna”** modules, the **ADD** module is powered by 5V (nominal) USB voltages. Pins along the top edge are placed and spaced to draw USB power from the top two *power rails* of a standard electronics breadboard. The circuitry of the **ADD** module consists of operational amplifiers (op-amps) to perform the adding of voltages—the USB power limits the output of the **ADD** module to within the 0V to 5V range.

Also like the most of the other **“Anna”** modules, the **ADD** module requires a “zero” reference of 2.50V. An input pin in the lower-left corner of the PCB is provided for this reference voltage to be supplied. The 2.50V is supplied to the non-inverting input of the op-amps that perform the operations and therefore acts as a *virtual ground*.

## Overload

Currently there is no **overload** detection on the **ADD** module itself. If the result of operating on an input voltage results in a voltage outside of the range of **“Anna's”** defined machine range of 1.0V and 4.0V, it is simply presented as is. The **MAIN** module has overload detection on all five of its source voltage inputs and is typically where you will detect an overload.

<div align="center">
  <img src="images/ADD Schematic.png" alt="ADD Schematic" width="795">
</div>

## Summing Junction

The schematic (above) shows four op-amps, their non-inverting inputs tied to virtual ground (“zero”, 2.50V). Adder **A** has three inputs and adder **B** has two—these are connected to the inverting inputs of the corresponding op-amps. In both cases, where the inputs tie together (known as the *summing junction*) and test-point is provided such that a user can join additional inputs if desired.

Input resistors on the **ADD** module are 100 kΩ. If similar resistor values are used for additional inputs at the summing junction, the extra inputs will be added normally. However, an input resistor value 10x (ten times) 100 kΩ (1 MΩ) will “weigh” the input by 0.1x (one-tenth). An input resistor that is 0.1x of 100 kΩ (10 kΩ) has the opposite weighting effect—the input is treated as 10x (ten times).

## Inverted Output

By connecting the inputs to be added to the inverting input of the op-amp, the result will also be inverted—the inverted sum of the input voltages. This is why the **ADD** module returns the negative sum (-SUM). I am not expert enough in electronics to understand fully why we use the inverting input for the summing junction.

From an LLM (since this is where it gets fuzzy for me), “The summing junction works because each voltage input behaves independently—seeing only its source voltage across its input resistor, with virtual ground on the other end. That virtual ground exists at the inverting input because the op-ampʼs feedback loop continuously drives it to match the actual ground connected to the non-inverting input. You canʼt simply swap the summing junction to the non-inverting input because virtual ground only exists at the inverting input—without it, the inputs interact with each other and the clean superposition breaks down.”

## Inverters

When analog computing, it turns out that inverting a value is common enough that the **ADD** module dedicates two op-amps just to take an input and return the inverted output. While any **ADD** operator will do the same with a single input, the dedicated inverters save on “breadboard real-estate” (number of pins).

## TODO

• Claude suggested adding a "pull to ZERO" resistor on every input. (1MΩ to 10MΩ)

• S.J. (summing junction) hole is the wrong size.
