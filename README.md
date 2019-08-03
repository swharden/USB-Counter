# USB Counter

<img align='right' height='300' src='https://raw.githubusercontent.com/swharden/USB-Counter/master/builds/1.1/photos/curved2.jpg'/>

This project contains the design files and notes for a USB counter. This device can measure RF (when the small signal amplifier front-end is populated) or TTL pulses (0-3.3V).

## Design Goals

**Precision frequency measurement:** One advantage of this counter is that it is never reset. Its core is a 32-bit counter, and every gate cycle transmits the current count to the computer over USB. Because every input cycle is measured high precision measurements of frequency over long periods of time are possible. For example, 1000 repeated measurements with a 1Hz gate allows frequency measurement to a precision of 0.01 Hz.

**Optional 7-segment display:** This device can be used without a computer, as it can be interfaced with a SPI-driven 8-digit 7-segment display module.

**Internal or external gating:** The microcontroller is capable of generating gate cycles in software. Precision is limited to that of the TCXO used to clock the microcontroller (2.5 PPM). For higher-precision gating a resistor may be lifted and an external gate applied (e.g., 1PPS GPS signal).

**USB Pulse Counter:** While counting pulses is not the primary design goal of this project, this device can serve as a USB pulse counter. The RF amplifier pathway can be omitted, and the input signal (0-3.3V pulses) be delivered directly into the counter chip.

## Builds

* [Revision 1.0](/builds/1.0) was built on 2019-07-26
* [Revision 1.1](/builds/1.1) was built on 2019-08-03
