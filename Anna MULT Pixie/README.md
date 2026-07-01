# MULT Module

The **MULT** module has two multiplier operators. Both take a pair of values assumed to be within *machine range* (between 1.0V and 4.0V). Each outputs the product of the two inputs. The outputs (products) from the **MULT** module are not inverted as they are for the **INT** and **ADD** module. This is because the **MULT** module wokrs very differently (as is discussed below).

### ZERO

Before going further though, like the other operator modules, *machine zero* (**ZERO**, 2.5V) is a required input. The usual way to do this is to connect a wire from the **MAIN** module's **ZERO** voltage output.

### Multiplication Operation

As stated, the two values to be multiplied are assumed to be between *-UNIT* and *+UNIT*. The multiplier operators treat this range of input values as though they represent -1.0 to +1.0. With -1.0 (*-UNIT*) being the smallest value and +1.0 (*+UNIT*) the largest value to be multiplied, it should be clear that, mathematically, the product too will always be within this range. For this reason, the **MULT** module should not ever be a source of *overflow*. (The product should only overflow if one of the multiplicands was already overflowing.)

<div align="center">
  <img src="images/MULT Schematic.png" alt="MULT Schematic" width="910">
</div>
