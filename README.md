# USB Counter

This project contains the design files and notes for a USB counter. By populating one of two input paths with components this device can serve as a sensitive frequency counter (with a small-signal amplifier front-end) or a pulse counter (with a simple 50 ohm load at the front-end). Measured frequency is sent to the computer over USB, but can also be displayed on an optional 7-segment display module.

![](/builds/1.1/photos/curved2.jpg)

### LED Display

In addition to serial output, this counter can optionally interface a SPI-driven 7-segment display (based on a MAX7219). These display modules are about [$4 on Amazon](https://www.amazon.com/s?k=max7219+segment) and allow this counter to be used without a computer. Note that the LED display and USB output can both be used at the same time.

![](/builds/1.1/photos/DSC_0132.jpg)

### USB (Serial) Output

When plugged in to a USB port the counter looks like a serial device. It continuously transmits the count (at 115200 baud) at an update rate which can be customized (commonly 1Hz, 10Hz, and 100Hz.) The output is two numbers: time (in seconds) and the count between the last update and this one. In this example it is measuring a radio frequency signal near 10.139977 MHz

![](/graphics/2019-08-04-output.png)

### Precision Frequency Measurement

One advantage of this counter is that it is never reset. Its core is a 32-bit counter, and every gate cycle transmits the current count to the computer over USB. Because every input cycle is measured high precision measurements of frequency over long periods of time are possible. For example, 1000 repeated measurements with a 1Hz gate allows frequency measurement to a precision of 0.01 Hz.

### Internal or External Gating
The microcontroller is capable of generating gate cycles in software. Precision is limited to that of the TCXO used to clock the microcontroller (2.5 PPM). For higher-precision gating a resistor may be lifted and an external gate applied (e.g., 1PPS GPS signal).

### Design and Build Notes

* [Revision 1.0](/builds/1.0) was built on 2019-07-26
* [Revision 1.1](/builds/1.1) was built on 2019-08-03

### About

This project was created by [Harden Technologies, LLC](http://tech.swharden.com). To inquire about the development special features or customized versions of this hardware or software, contact the author at [SWHarden@gmail.com](mailto:swharden@gmail.com).
