# Rotory Encoder for arduino

Description comming soon.

## Licence

This software is licened under the MIT licence

Copyright (c) 2021 Mats van Reenen

## Pinout

### Arduino Uno

| pin | goes to             |
|----:|---------------------|
|   2 | light gate 0        |
|   3 | light gate 1        |
|  10 | home switch         |
|  11 | analog out (filter) |

The analog out is a PWM signal of 31 kHz it you add a low pass filter 1 k&Omega; en 1 nF.