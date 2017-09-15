/***********************************************************/
/*   ____ ___ _   _ ___ ___     ____ _                     */
/*  |  _ \_ _| | | |_ _/ _ \   / ___| | ___  _ __   ___    */
/*  | |_) | || | | || | | | | | |   | |/ _ \| '_ \ / _ \   */
/*  |  __/| || |_| || | |_| | | |___| | (_) | | | |  __/   */
/*  |_|  |___|\___/|___\___/   \____|_|\___/|_| |_|\___|   */
/*                                                         */
/*  By: Lucas Teske (USB-ON-AT90USBXXX BY CKDUR)           */
/***********************************************************/
/*   Basicly this is an PIUIO Clone with an ATMEGA8U2 and  */
/*    serial from ATMEGA328 interfaced on ARDUINO UNO      */
/***********************************************************/
/*      This is main code from ATMEGA328 pool system       */
/***********************************************************/
/*                    License is WHAT?                     */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

#ifndef UNOJOY_H
#define UNOJOY_H
    #include <stdint.h>
    #include <util/atomic.h>
    #include <Arduino.h>
	
	// CKDUR: Maybe we need this after
        unsigned long  tick;
        unsigned char tmp1;	
        unsigned char i;
	
	// RACERXL: Some Macros to help
	#define GETBIT(port,bit) ((unsigned char)(((unsigned char)(port)) & ((unsigned char)(0x01 << ((unsigned char)(bit))))))
	#define SETBIT(port,bit) port |= (0x01 << (bit))    // RACERXL:   Set Byte bit
	#define CLRBIT(port,bit) port &= ~(0x01 << (bit))   // RACERXL:   Clr Byte bit

	static unsigned char Input[2];          // RACERXL:   The actual 16 bits Input data
	static unsigned char Output[3];         // RACERXL:   The actual 16 bits Output data
	
    // Call setupArdPIUino in the setup block of your program.
    //  It sets up the hardware UnoJoy needs to work properly
    void setupArdPIUino(void);
    
    // You can also call the set
    void setupArdPIUino(int);
    
//----- End of the interface code you should be using -----//
//----- Below here is the actual implementation of
  
  // serialCheckInterval governs how many ms between
  //  checks to the serial port for data.
  //  It shouldn't go above 20 or so, otherwise you might
  //  get unreliable data transmission to the UnoJoy firmware,
  //  since after it sends a request, it waits 25 ms for a response.
  //  If you really need to make it bigger than that, you'll have to
  //  adjust that timeout in the UnoJoy ATmega8u2 firmware code as well.
  volatile int serialCheckInterval = 1;
  // This is an internal counter variable to count ms between
  //  serial check times
  int serialCheckCounter = 0;
  
  // This is the setup function - it sets up the serial communication
  //  and the timer interrupt for actually sending the data back and forth.
  void setupArdPIUino(void){
  
    // Start the serial port at the specific, low-error rate UnoJoy uses.
    //  If you want to change the rate, you'll have to change it in the
    //  firmware for the ATmega8u2 as well.  250,000 is actually the best rate,
    //  but it's not supported on Macs, breaking the processing debugger.
    Serial.begin(38400);
    
    // Now set up the Timer 0 compare register A
    //  so that Timer0 (used for millis() and such)
    //  also fires an interrupt when it's equal to
    //  128, not just on overflow.
    // This will fire our timer interrupt almost
    //  every 1 ms (1024 us to be exact).
    OCR0A = 128;
    TIMSK0 |= (1 << OCIE0A);
  }
  
  // If you really need to change the serial polling
  //  interval, use this function to initialize UnoJoy.
  //  interval is the polling frequency, in ms.
  void setupArdPIUino(int interval){
    serialCheckInterval = interval;
    setupArdPIUino();
  }
  
  // This interrupt gets called approximately once per ms.
  //  It counts how many ms between serial port polls,
  //  and if it's been long enough, polls the serial
  //  port to see if the UnoJoy firmware requested data.
  //  If it did, it transmits the appropriate data back.
  ISR(TIMER0_COMPA_vect){
    serialCheckCounter++;
    if (serialCheckCounter >= serialCheckInterval){
      serialCheckCounter = 0;
      // If there is incoming data stored in the Arduino serial buffer
      while (Serial.available() > 0) {
        //pinMode(13, OUTPUT);
        //digitalWrite(13, HIGH);
        // Get incoming byte from the ATmega8u2
        byte inByte = Serial.read();
		
		// CKDUR: That number tells us which byte 
		if(inByte < 2)
                {
	            Serial.write(Input[inByte]);
                }
        else if(3 == inByte)
        {
          for(i = 0; i < 3; i++)
          {
              tick = 0;
              while (Serial.available() <= 0 && tick < 10) tick++;
              if(tick >= 10) continue;
              Output[i] = Serial.read();
          }
        }
			
        //digitalWrite(13, LOW);
      }
    }
  }

#endif
