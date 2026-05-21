# MAIN

## Software

**MAIN** features a *daughter board* consisting of an ESP32 Dev Board that includes an LCD display and two soldered-on pushbuttons. Software written for **MAIN** makes use of the display and buttons.

## Metering Modes

The bottom-right button, the one closest to the user, Iʼll call the *mode button*, was chosen to cycle through **MAIN**s various metering modes. The other button, Iʼll call the *options button*, often cycles through various options for the selected metering mode.

### Vertical Meters

The default metering mode consists of five moveable, vertical meters and a fixed red index line. They are labeled 1 though 5 and correspond to the inputs on **MAIN***, **SRC1** through **SRC5**. The *options button* allows you to toggle the number of inputs displayed from the 5 down to just 1.

### Sweep Meters

### Strip Chart

<div align="center">
<img src="images/Anna (MAIN, Lotka-Volterra Strip Chart).png" alt="Strip Chart screenshot" width="240">
</div>

The Strip Chart is a moving metering mode that can display all 5 source inputs with roughly 4 seconds of history visible. The vertical rules that scroll by mark off 1 second intervals. The *options button*  allows you to toggle the number of inputs displayed. *Overload* is represented by the lables flashing.

### XY Scope

<div align="center">
<img src="images/Anna (MAIN, Lotka-Volterra XY Scope).png" alt="XY Scope screenshot" width="240">
</div>

### Spring + Mass

<div align="center">
<img src="images/Anna (MAIN, Spring + Mass).png" alt="Spring + Mass screenshot" width="240">
</div>

### Lunar Lander

<div align="center">
<img src="images/Anna (MAIN, Lunar Lander).png" alt="Lunar Lander screenshot" width="240">
</div>

### Raw Values

<div align="center">
<img src="images/Anna (MAIN, Raw Values).png" alt="Raw Values screenshot" width="240">
</div>

Initially kind of a debug mode, it displays the raw values of the inputs coming off the ADCs (Analog to Digital chips). In addition to **SRC1** through **SRC5** though, it also shows the measured values for **+UNIT**, **ZERO** and **-UNIT**.

When a source input is indicated as Overloading, it is because it is outside the range of the measured **+UNIT** and **-UNIT**. It is indicated in this mode when the inputʼs label is rendered white. **+UNIT**, **ZERO** and **-UNIT**, by definition, can never be “overloading”.

When a source input is displayed on the LCD, as a reading on a meter for example, it is arrived at by finding where the source input value is proportionally between the reference values (voltages). If, for example, the value is greater than **ZERO**, itʼs relative value is determined by scaling it proportionally between the measured **ZERO** and **+UNIT**.
