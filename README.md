# USB Counter

This project folder contains the design, notes, and experiments relating to a USB frequency counter (which can be configured as a pulse counter). USB connectivity is ideal for data-logging applications where counts are to be monitored over time. 

Like other counter designs this one has an _input_ and a _gate_. Every time the gate goes high, the count is transmitted to the computer. Counts "roll over" from one gate cycle to the next so no cycle is ever lost. This allows high-precision measurements given long periods of recording time.

If you don't have a precision 1Hz gate available, you're still covered! This device has a 10MHz temperature-compensated crystal oscillator which can be divided-down (in software) and used as a 1Hz gate.

**Project status: Alpha** - this project is in active development, and designs are not yet ready for serious public assessment.

![](graphics/2019-07-25.jpg)