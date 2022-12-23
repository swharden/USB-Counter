# Version 2.2

## Thoughts

* Make an RF amplifier board separately (perhaps with prescaler)
* Input should be 0-5V TTL
* What makes this unique is no-cycle-lost counting
* Ability to accept a 10 MHz lab reference clock is also a big win

## Build notes
* Red and blue LEDs need 1K resistors
* Green LEDs need 47k resistors
* Capacitors on USB line should be 47pF
* LJCPCB blue silkscreen is ugly and doesn't hold up to heat as well as green
* For the crystal oscillator indicate where the dot goes
* It's annoying to keep rotating the board when hand assembling so lay-out passives horizontally
* Move the board ID under one of the ICs
* Consider horizontally mounted headers and SMA connector to facilitate stacking
* Clamp diodes should be downstream of the input decoupling decapacitors

## Schematic

![](schematic.png)

## Alternative Frontend Designs

### TLV3501 RF Comparator
![](comparator-TLV3501-frontend-idea.png)

### OPA356 RF Opamp
![](opamp-opa356-frontend-idea.png)
