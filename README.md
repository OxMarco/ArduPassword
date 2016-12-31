# ArduPassword
Generates passwords of any given length l<16 because of the LCD columns available just using an Arduino Uno and a 16x2 LCD module. A button allows to recreate a new password whenever needed and a led blinks when one was successfully created.
To increase security, an internal timer resets the password every 10 seconds and the password itself is never stored but immediately overwritten by zeroes.
A watchdog prevents the microcontroller from hanging and showing the last prompted password.

## Parts list
- 1x Arduino Uno
- 1x LCD 16x2
- 1x LED
- 1x push button
- 1x 10 kΩ resistor
- 2x 220 Ω resistors
- 1x 10 kΩ potentiometer
- jumpers, USB cable and breadboard

More here: https://circuits.io/circuits/3614610-ardupassword
