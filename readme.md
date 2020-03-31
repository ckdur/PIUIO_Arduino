# Arduino Uno PIUIO

Emulates a Pump it Up IO board (a.k.a. MK6 PIUIO) using an Arduino Uno.
Tested and working with both input and lighting with Pump it Up Prime 2015, Stepmania 5 on Linux with djpohly's input driver, and on Windows with IO2KEY.


## Install
First:
1. Upload ArduinoPIUAux (or ArduinoPIUAux_Lights for lighting support) onto your Arduino Uno (as you would a regular Arduino sketch)

In linux ([WSL works too if you're on Windows 8/10](https://www.microsoft.com/en-us/p/debian/9msvkqc78pk6?activetab=pivot:overviewtab)):
1. `sudo apt-get install build-essential git gcc-avr avr-libc`
2. `git clone https://github.com/48productions/PIUIO_Arduino`
3. cd to the cloned folder, delete the lufa folder
4. `git clone https://github.com/abcminiuser/lufa`
5. `make`
6. Move PIUIO.hex,elf,bin,eep,lss,map,sym into ATmega8u2Code/HexFiles

Then reboot into windows...
1. Install FLIP https://www.microchip.com/developmenttools/ProductDetails/flip
2. Plug in the Arduino UNO
3. Note down the port you plugged it into in device manager, you will need it
3. Put the UNO into DFU mode - With the Uno oriented with the USB port to the left, briefly bridge the left two pins on the 2x3 header near the USB port.
4. Install the device by going into device manager, finding the port you noted down earlier, then selecting from a list of drivers, have disk, naviate to "Program Files (x86)/Atmel/Flip 3.4.7/usb/" and then double click the inf
5. After the drivers are set up correctly, run TurnIntoAPIUIO.bat. Congrats, your Arduino is now a PIUIO!

Note that you can't upload new sketches to your Arduino when it's acting as a PIUIO. If you ever need to change your Arduino back into an Arduino to upload new code:
1. Navigate to C:\Program Files (x86)\Arduino\hardware\arduino\avr\firmwares\atmegaxxu2\arduino-usbserial
2. Copy Arduino-usbserial-uno.hex into HexFiles
3. Put your arduino into DFU mode and run the batch script. Once this above file is copied, you just need to put the arduino in DFU mode and run the appropriate batch file to change between Arduino/PIUIO modes.
