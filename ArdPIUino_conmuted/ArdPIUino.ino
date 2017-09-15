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
  setupPins();
  setupArdPIUino();
}

unsigned char inputn,tmp2, tmp3; 
void loop(){
	// CKDUR: This is void pollInputOutput()
	
	//    This will set outputs and get inputs
    //    The board will use 4067 muxer that is 16-to-1 muxer.
    //    PORTC is the Muxer Selector. (AKA A0 to A3)
    //    PORTB0 is the Muxer Output (AKA D8)
       
    digitalWrite(12, HIGH);                                                        //    Disable the latches input
    for(inputn=0;inputn<16;inputn++)    {
        //PORTC = inputn;                                                     //    Sets the muxer position
        digitalWrite(A4, GETBIT(inputn, 0)?HIGH:LOW);
        digitalWrite(A5, GETBIT(inputn, 1)?HIGH:LOW);
        digitalWrite(A2, GETBIT(inputn, 2)?HIGH:LOW);
        digitalWrite(A3, GETBIT(inputn, 3)?HIGH:LOW);
        delay(10);
        tmp1 = digitalRead(8)?0:1;                                              //    Gets the input
        if(tmp1)
            SETBIT(Input[inputn/8],inputn%8);                          //    Sets if input = 1
        else
            CLRBIT(Input[inputn/8],inputn%8);                          //    Clears if input = 0
    }
    //    The board will use 4099 as output latch. Two are used to get 16 bit output.
    //    PORTC is the address, PORTB1 is Data for First byte, PORTB2 is Data for Second, PORTB3 is Data for Third (AKA D9 TO D11)
    //    The Latch input enables are in PORTB4 (D12)
    //    So we will set two bits per loop cycle
    digitalWrite(12, LOW);                                                       //    Enable the latches input
    for(inputn=0;inputn<8;inputn++)    {
        digitalWrite(A4, GETBIT(inputn, 0)?HIGH:LOW);
        digitalWrite(A5, GETBIT(inputn, 1)?HIGH:LOW);
        digitalWrite(A2, GETBIT(inputn, 2)?HIGH:LOW);
        digitalWrite(A3, GETBIT(inputn, 3)?HIGH:LOW);                        //    Sets the address
        
        tmp1 = GETBIT(Output[0],inputn);                                    //    Gets the output data
        tmp2 = GETBIT(Output[1],inputn);
        tmp3 = GETBIT(Output[2],inputn);             //    CKDUR: This is mine
        digitalWrite(9, tmp1?LOW:HIGH);              //    first latch
        digitalWrite(10, tmp2?LOW:HIGH);             //    second latch
        digitalWrite(11, tmp3?LOW:HIGH);             //    CKDUR: This is mine, third latch
        
    }
    digitalWrite(12, HIGH);                                                        //    Disable the latches input, just in case.
}

void setupPins(void){
	// CKDUR: For Programmer POV, this is easier
    //DDRC = 255;
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    //PORTC = 0;
    //DDRB = 0b00111110;
    pinMode(8, INPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
}
