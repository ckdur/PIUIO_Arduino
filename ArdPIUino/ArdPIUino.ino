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

#include "ArdPIUino.h"

void setup(){
  Input[0] = 0xFF;
  Input[1] = 0xFF;
  setupPins();
  setupArdPIUino();
}

unsigned char inputn,tmp2, tmp3; 
#define SET_THING if(tmp1) SETBIT(Input[inputn/8],inputn%8); else CLRBIT(Input[inputn/8],inputn%8);
void loop(){
	// CKDUR: This is void pollInputOutput()
  inputn = 4; tmp1 = digitalRead(2); SET_THING
  inputn = 3; tmp1 = digitalRead(3); SET_THING
  inputn = 2; tmp1 = digitalRead(4); SET_THING
  inputn = 1; tmp1 = digitalRead(5); SET_THING
  inputn = 0; tmp1 = digitalRead(6); SET_THING
  inputn = 12; tmp1 = digitalRead(7); SET_THING
  inputn = 11; tmp1 = digitalRead(8); SET_THING
  inputn = 10; tmp1 = digitalRead(9); SET_THING
  inputn = 9; tmp1 = digitalRead(10); SET_THING
  inputn = 8; tmp1 = digitalRead(11); SET_THING
 // inputn = 5; tmp1 = digitalRead(A1); SET_THING // CLEAR
  //inputn = 6; tmp1 = digitalRead(13); SET_THING // SERVICE
  inputn = 8+5; tmp1 = digitalRead(12); SET_THING // TEST
  //inputn = 7; tmp1 = digitalRead(A4); SET_THING // COIN 1 ?
  //inputn = 8+7; tmp1 = digitalRead(A5)?0:1; SET_THING // COIN 2 ?
        
}

void setupPins(void){
	// CKDUR: For Programmer POV, this is easier
    //DDRC = 255;
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
    //PORTC = 0;
    //DDRB = 0b00111110;
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
}
