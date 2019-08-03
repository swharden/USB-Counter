# USB Frequency Counter (rev 1.1)

I ordered this PCB on 2019-07-28

![](pcb-rndr.png)

### PCB Design
![](pcb-dsn.png)

### Schematic
![](schematic.png)

## Changes from rev 1.0
* improved RF amplifier design
* alternate path to bypass RF amplifier
* corrected counter pin connections
* free a MCU pin by making the status LED the gate
* add a header for the serial LED display
* run MCU at 14.7456 MHz
  * allows much faster serial transmission
  * no longer accepts external 10MHz reference
  * may still accept external gate
* added screw holes
  * they're floating (not grounded) - should they be grounded?
* switched to micro USB (from mini USB)

## Components
* all passives are 0805 (~$1)
* [SBAV99WT1G SC-70 dual diode](https://www.mouser.com/ProductDetail/ON-Semiconductor/SBAV99WT1G?qs=%2Fha2pyFaduhs9dhfVWP8oT%252BsAj5t0ZSYddkb6PuTtd0%3D) 215 mA ($0.29)
* [14.7456MHz TCXO](https://www.mouser.com/ProductDetail/Fox/FOX924B-147456?qs=sGAEpiMZZMt8oz%2FHeiymADfzZKRiEXclvcmWd5jLzoM%3D) 2.5 PPM, 14.7456MHz ($2.36)
* [SN74LV8154](https://www.mouser.com/ProductDetail/Texas-Instruments/SN74LV8154PWR?qs=sGAEpiMZZMtdY2G%252BSI3N4aQvQNXOTGN6Ghdjz%252BkScFE%3D) ($0.99) TSSOP-20
* [FT230XS-R](https://www.mouser.com/ProductDetail/FTDI/FT230XS-R?qs=sGAEpiMZZMtv%252Bwxsgy%2FhiIaF6qCroMVR1i2pEQA5UpU%3D) ($2.04) SSOP-16
* [ATMega328](https://www.mouser.com/ProductDetail/Microchip-Technology-Atmel/ATMEGA328PB-AU?qs=sGAEpiMZZMvc81WFyF5EdrSRAEYMYvHlMc95YQj%2FArE%3D) ($1.38)
* [mini-USB jack](https://www.mouser.com/ProductDetail/CUI/UJ2-MBH-1-SMT-TR?qs=sGAEpiMZZMu3xu3GWjvQiLfiCTO8RP%252Bk%252BIiwpoT5qew%3D) ($0.49)
* [micro-USB jack](https://www.mouser.com/ProductDetail/Hirose-Connector/ZX62D-B-5PA830?qs=sGAEpiMZZMulM8LPOQ%252Byk6r3VmhUEyMLT8hu1C1GYL85FtczwhvFwQ%3D%3D) ($0.70)
* [SMA connector](https://www.mouser.com/ProductDetail/LPRS/SMA-CONNECTOR?qs=sGAEpiMZZMuLQf%252BEuFsOrkd7M7rmHNHidLMZ%2Ftb%252B0T1YCJLScw0qLA%3D%3D) ($1.08)
* [SPI-driven 8-digit 7-segment display module](https://www.amazon.com/dp/B07CL2YNJQ) ($13 for 4)


## Build Notes
* didnt have 27 ohm resistors. used 22 ohm.
* I used a 500mW rated R15
* make hand solder version of usb
* make oscillator fit less awkwardly

![](photos/DSC_0098.JPG)
![](photos/curved.jpg)
![](photos/DSC_0100.JPG)
![](photos/curved2.jpg)