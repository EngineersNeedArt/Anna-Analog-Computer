# MAIN Module

## Software

**MAIN** features a *daughter board* consisting of an ESP32 Dev Board that includes an LCD display and two soldered-on pushbuttons. Software written for **MAIN** makes use of the display and buttons.

## Metering Modes

The bottom-right button, the one closest to the user, Iʼll call the *mode button*, was chosen to cycle through **MAIN**s various metering modes. The other button, Iʼll call the *options button*, often cycles through various options for the selected metering mode.

### Vertical Meters

<div align="center">
<img src="images/Anna (MAIN, Vertical Meters).png" alt="Vertical Meters screenshot" width="240">
</div>

The default metering mode consists of five moveable, vertical meters and a fixed red index line. They are labeled 1 though 5 and correspond to the inputs on **MAIN**, **SRC1** through **SRC5**. The *options button* allows you to toggle the number of inputs displayed from the 5 down to just 1. Overload is represented by the labels flashing.

### Sweep Meters

<div align="center">
<img src="images/Anna (MAIN, Sweep Meters).png" alt="Sweep Meters screenshot" width="240">
</div>

Looking like traditional analog panel meters, the magic of digital emulation means that 5 needles can share the same meter. They are labeled 1 though 5, color coded, and correspond to the inputs on **MAIN**, **SRC1** through **SRC5**. The *options button* allows you to toggle the number of inputs displayed from the 5 down to just 1. Overload is represented by the labels flashing.

***(NOTE: in the screenshot only 3 inputs are enabled.)***

### Strip Chart

<div align="center">
<img src="images/Anna (MAIN, Lotka-Volterra Strip Chart).png" alt="Strip Chart screenshot" width="240">
</div>

The Strip Chart is a moving metering mode that can display all 5 source inputs with roughly 4 seconds of history visible. The vertical rules that scroll by mark off 1 second intervals. The *options button*  allows you to toggle the number of inputs displayed. *Overload* is represented by the labels flashing.

***(NOTE: in the screenshot only 2 inputs are enabled.)***

### XY Scope

<div align="center">
<img src="images/Anna (MAIN, Lotka-Volterra XY Scope).png" alt="XY Scope screenshot" width="240">
</div>

The XY Scope is patterned after an oscilloscope. It makes use of **SRC1** to control the X-position of a dot and **SRC2** to control the Y-position. The *options button* toggles between displaying the whole *machine value* range to displaying just Quadrant I (the positive *machine values*). The *options button* also toggles between two *fade speeds* for the phosphor trace: slow and fast.

### Spring + Mass

<div align="center">
<img src="images/Anna (MAIN, Spring + Mass).png" alt="Spring + Mass screenshot" width="240">
</div>

Spring + Mass is a special demonstration metering mode. It represents a mass on a spring. The vertical position of the mass is controlled solely by the voltage at **SRC1**. The *options button* does nothing.

The mass leaves a “trail” sort of like the Strip Chart. A color change indicates 1-second intervals.

### Lunar Lander

<div align="center">
<img src="images/Anna (MAIN, Lunar Lander).png" alt="Lunar Lander screenshot" width="240">
</div>

Lunar Lander is another special metering mode that is a game. It expects a gravitational constant to be presented at **SRC1**, the intermittent thrust to be connected to **SRC2**, the spacecraft velocity to be connected to **SRC3** and the spacecraft elevation/altitude to be connected to **SRC4**. An optional *fuel level* voltage can be connected to **SRC5**.

The above connections presume that the user has wired up the lunar lander program on the analog computer—supplied the above voltages as described.

Starting the game is done by simply resetting the integrators that dictate velocity (reset to **ZERO**), altitude (reset to **+UNIT**) and, if the fuel level integrator is wired up, resetting the fuel level to **+UNIT**. Doing so put the spacecraft at the top fo the display, with no vertical velocity, and optionally with a full tank of fuel.

As the spacecraft descends, the software monitors the spacecraft altitude (**SRC4**) to determine touch-down. When the spacecraft altitude passes the touch-down threshold, velocity (**SRC3**) is evaluated by the software to determine if the landing was safe. The **MAIN** display will indicate a safe landing or a crash.

If the optional fuel level (**SRC5**) ever drops to an empty tank, the game will indicate a crash landing.

### Raw Values

<div align="center">
<img src="images/Anna (MAIN, Raw Values).png" alt="Raw Values screenshot" width="240">
</div>

Initially kind of a debug mode, it displays the raw values of the inputs coming off the ADCs (Analog to Digital chips). In addition to **SRC1** through **SRC5** though, it also shows the measured values for **+UNIT**, **ZERO** and **-UNIT**.

When a source input is indicated as Overloading, it is because it is outside the range of the measured **+UNIT** and **-UNIT**. It is indicated in this mode when the inputʼs label is rendered white. **+UNIT**, **ZERO** and **-UNIT**, by definition, can never be “overloading”.

When a source input is displayed on the LCD, as a reading on a meter for example, it is arrived at by finding where the source input value is proportionally between the reference values (voltages). If, for example, the value is greater than **ZERO**, itʼs relative value is determined by scaling it proportionally between the measured **ZERO** and **+UNIT**.

## Hardware

<div align="center">
<img src="images/MAIN Schematic.png" alt="MAIN Schematic" width="1480">
</div>

The ESP32 dev board (LILYGO T-Display) dominates the schematic with the large yellow box in the upper right. It has the USB-C connection from which the entire **“Anna”** ecosystem derives USB power. A pair of capacitors serve to smooth the current on those power rails.

### Buffering

Across the bottom are eight op-amps involved in buffering (wired as voltage followers). Three are used to buffer the outputs of the machine values/voltages (1.0V, 2.5V and 4.0V)—in this way the voltages are protected from being loaded down by the analog circuitry.

The other five op-amps are used to buffer the five source-voltage inputs to **MAIN**. 

### ADC

Just above the op-amps are a pair of ADC chips (analog-to-digital). Each chip has four channels for a total of eight inputs. Three of these are dedicated to converting the machine values/voltages (1.0V, 2.5V and 4.0V) to digital form. The other five are, unsurprisingly, used to convert the source-voltage inputs to digital form.

The ESP32 reads the 8 digital values over I2C.

*Note: the ESP32 works on 3.3V and can be damaged by the 5V that **“Anna”** uses. The ADC chips are running from 5V and can therefore directly interface **“Anna's”** 5V inputs. To interface between the ADC chips and the ESP32, a dedicated chip handles level-shifting the I2C connection.*

When, for example, the ESP32 is to display a source-voltage input as a needle on a meter, it can use the three digital machine values to find a proportional position to move the needle to.

Further, if any digital source-voltage input exceeds the digital positive machine unit value or is less than the digital negative machine unit value, it is flagged as **overloading**.

### Voltage Sources

A last point to call out is the small component on the far-left (U5). It takes the voltage from USB and provides the stable 4.0V that **“Anna”** uses for positive machine value. The other machine values (zero and negative unit), are pared from the 4.0V using carefully selected resistor values arranged as voltage dividers.
