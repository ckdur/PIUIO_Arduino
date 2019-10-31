# Arduino Uno PIUIO

Emulates a Pump it Up IO board using an Arduino Uno.
Tested and working with both input and lighting with Pump it Up Prime 2015 as well as Stepmania 5 on Linux with djpohly's input driver.


## Install
First:
1. Upload ArdPIUino onto your Arduino Uno (as you would a regular Arduino sketch)

In linux:
1. `sudo apt-get install gcc-avr avr-libc`
2. delete lufa folder
3. `git clone https://github.com/abcminiuser/lufa`
4. `make`
5. Move PIUIO.hex,elf,bin,eep,lss,map,sym into ATmega8u2Code/HexFiles

Then reboot into windows...
1. Install FLIP https://www.microchip.com/developmenttools/ProductDetails/flip
2. Plug in the Arduino UNO
3. Note down the port you plugged it into in device manager, you will need it
3. Put the UNO into DFU mode - With the Uno oriented with the USB port to the left, briefly bridge the left two pins on the 2x3 header near the USB port.
4. Install the device by going into device manager, finding the port you noted down earlier, then selecting from a list of drivers, have disk, naviate to "Program Files (x86)/Atmel/Flip 3.4.7/usb/" and then double click the inf
5. After the drivers are set up correctly, run TurnIntoAPIUIO.bat

If you ever need to change your Arduino back into an Arduino:
1. Navigate to C:\Program Files (x86)\Arduino\hardware\arduino\avr\firmwares\atmegaxxu2\arduino-usbserial
2. Copy Arduino-usbserial-uno.hex into HexFiles
3. Put your arduino into DFU mode and run the batch script
