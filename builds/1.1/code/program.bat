:: ATMEGA328P COMPILE (AVG-GCC) AND PROGRAM (AVRDUDE/USBTINY) SCRIPT
:: by Scott Harden / Harden Technologies, LLC

@echo off

:: delete old files if they exist
IF EXIST "main.elf" (
    DEL main.elf
    ECHO deleted old ELF file
)
IF EXIST "main.hex" (
    DEL main.hex
    ECHO deleted old HEX file
)

:: compile C code into a GNU binary file
ECHO compiling...
avr-gcc -mmcu=atmega328p -std=c99 -Wall -Os -o main.elf main.c -w
IF EXIST "main.elf" (
    ECHO verified ELF file is present.
) ELSE (
    ECHO ERROR: ELF FILE NOT CREATED!
)

:: convert the binary file into a hex file
ECHO creating HEX file ...
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
IF EXIST "main.hex" (
    ECHO.
) ELSE (
    ECHO FAILED!
    EXIT
)

:: copy the hex file onto the microcontroller using a USB programmer (-q?)
ECHO programming microcontroller ...
ECHO.
avrdude -c usbtiny -p m328p -U flash:w:"main.hex":a -F