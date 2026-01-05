# Arduino Uno PIUIO

Emulates a Pump it Up IO board (either PIUIO or LXIOv1/2) using an Arduino Uno.
Tested and working with:
- Pump It Up Prime 2015 (PIUIO)
- Pump It Up Prime2 2017 (PIUIO/LXIOv1)
- Pump It Up XX 2019 (PIUIO/LXIOv1/LXIOv2)
- Pump It Up Phoenix 2024 (PIUIO/LXIOv1/LXIOv2)
- Stepmania5 on Linux with djpohly's input driver (PIUIO)
- [IO2KEY](https://github.com/ckdur/piuio2key) (PIUIO)

## Install
First:
1. Upload `ArduinoPIUAux` (or `ArduinoPIUAux_Lights` for lighting support) onto your Arduino
Uno (as you would a regular Arduino sketch)

In linux ([WSL works too if you're on Windows 8/10](https://www.microsoft.com/en-us/p/debian/9msvkqc78pk6?activetab=pivot:overviewtab)), execute the following:

```bash
sudo apt-get install build-essential git gcc-avr avr-libc dfu-programmer
git clone https://github.com/ckdur/PIUIO_Arduino
cd PIUIO_Arduino
git submodule update --init --recursive
# For Arduino UNO (Put it in DFU mode)
make
sudo make program
# For Arduino Leonardo
make -f makefile.leonardo
make -f makefile.leonardo program
```

In windows (Cannot compile):
1. Install FLIP https://www.microchip.com/developmenttools/ProductDetails/flip
2. Plug in the Arduino UNO
3. Note down the port you plugged it into in device manager, you will need it
3. Put the UNO into DFU mode - With the Uno oriented with the USB port to the left, briefly bridge the left two pins on the 2x3 header near the USB port.
4. Install the device by going into device manager, finding the port you noted down 
earlier, then selecting from a list of drivers, have disk, naviate to 
`Program Files (x86)/Atmel/Flip 3.4.7/usb/` and then double click the inf file.
5. After the drivers are set up correctly, run TurnIntoAPIUIO.bat. Congrats, 
your Arduino is now a PIUIO!

Note that you can't upload new sketches to your Arduino when it's acting as a PIUIO. If you ever need to change your Arduino back into an Arduino to upload new code:
1. Navigate to `C:\Program Files (x86)\Arduino\hardware\arduino\avr\firmwares\atmegaxxu2\arduino-usbserial`
2. Copy Arduino-usbserial-uno.hex into HexFiles
3. Put your arduino into DFU mode and run the batch script `TurnIntoAnArduino.bat`. Once 
this above file is copied, you just need to put the arduino in DFU mode and run the 
appropriate batch file to change between Arduino/PIUIO modes.

## Changing to LXIO mode

NOTE: The pin is `13` instead of `A0` if you used the `ArduinoPIUAux_Lights` sketch.
NOTE2: The pin is `0` instead of `A0` if you are using Arduino Leonardo.

The `PIUIO_Arduino` now supports LXIO mode. To change the device, just ground pin `A0`.
Everytime you ground the pin `A0`, you will change the driver internally. Once you do,
disconnect and reconnect the board. The USB shall change in the following order:

```
PIUIO (0543:1002) -> LXIOv1 (0d2f:1020) -> LXIOv2 (0d2f:1040) -> [Then goes back]
```

## Hate arduino?

Checkout [true-lxio-pico](https://github.com/ckdur/true-lxio-pico). This one works
in a raspberry pico.

## Others

Checkout [piuio2key](https://github.com/ckdur/piuio2key). Now with LXIO support.
