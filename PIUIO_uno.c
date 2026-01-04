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
/*          This is main code from PIUIO Clone             */
/***********************************************************/
/*          Please look at LICENSE for details             */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

#include "PIUIO.h"
#include "PIUIO_ctrl.h"

#define PIUIO_CTL_REQ 0xAE
#define RXLED 4
#define TXLED 5

// RACERXL: Some Vars to help
#define GETBIT(port,bit) ((unsigned char)(((unsigned char)(port)) & ((unsigned char)(0x01 << ((unsigned char)(bit))))))
#define SETBIT(port,bit) port |= (0x01 << (bit))    // RACERXL:   Set Byte bit
#define CLRBIT(port,bit) port &= ~(0x01 << (bit))   // RACERXL:   Clr Byte bit

static unsigned char Input[2];
static unsigned char Output[3];
static unsigned char i, tmp1;

// This turns on one of the LEDs hooked up to the chip
void LEDon(char ledNumber){
    DDRD |= 1 << ledNumber;
    PORTD &= ~(1 << ledNumber);
}

// And this turns it off
void LEDoff(char ledNumber){
    DDRD &= ~(1 << ledNumber);
    PORTD |= 1 << ledNumber;
}

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    nControl = 0;
    SetupHardware();

    // Start up the USART for serial communications
    // 25 corresponds to 38400 baud - see datasheet for more values
    //USART_Init(25);// 103 corresponds to 9600, 8 corresponds to 115200 baud, 3 for 250000
    Serial_Init(38400, false);

    GlobalInterruptEnable();

    for(i=0;i<8;i++){
        InputData[i] = 0xFF;
        LampData[i] = 0x0;

    }

    for (;;)
    {
RESTART:
        USB_USBTask();
        
        if(nControl > 1000) LEDon(RXLED);
        if(nControl > 2000) {LEDoff(RXLED); nControl = 0;}

        Output[0] = LampData[0];           //    The AM use unsigned short for those.
        Output[1] = LampData[2];           //    So we just skip one byte
        Output[2] = LampData[3];           //    CKDUR: But we can't skip 3rd
        tmp1 = GETBIT(LampData[1],2);       //     CKDUR: Also, we can't skip NEON data from 1st
        if(tmp1 > 0) {SETBIT(Output[2],7);} else {CLRBIT(Output[2],7);};
        
        char tmp;
        int count = 0;
        unsigned char m[2];
        Serial_SendByte(0x56);
        while(!Serial_IsCharReceived())
        {
            count++;
            if(count >= 10000) goto RESTART;
        }
        while(Serial_IsCharReceived())
         {
            tmp = Serial_ReceiveByte();
            if(tmp == 0xAE)
            {
                // CKDUR: Write Lamp Buffer
                for(i = 0; i < 3; i++)
                {
                    Serial_SendByte(Output[i]);
                }
                
                for(i = 0; i < 2; i++)
                {
                    
                    while(!Serial_IsCharReceived())
                    {
                        count++;
                        if(count >= 10000) goto RESTART;
                    }
                    m[i] = Serial_ReceiveByte();
                }
            }
        }
        
        Input[0] = m[0];
        Input[1] = m[1];
        
        if(m[0] != 0xFF || m[1] != 0xFF) LEDon(TXLED); else LEDoff(TXLED);

        // CKDUR: I'll put some bits from the input
        // and adapt it to current InputData
        // For coin & service

        /*for(i=0;i<8;i++){
            InputData[i] = 0xFF;
        }*/
        // Clear button
        tmp1 = GETBIT(Input[0],5); SETBIT(Input[0],5);
        if(!tmp1) {CLRBIT(InputData[1],7);} else {SETBIT(InputData[1],7);};
        if(!tmp1) {CLRBIT(InputData[3],7);} else {SETBIT(InputData[3],7);};
        // Service button
        tmp1 = GETBIT(Input[0],6); SETBIT(Input[0],6);
        if(!tmp1) {CLRBIT(InputData[1],6);} else {SETBIT(InputData[1],6);};
        if(!tmp1) {CLRBIT(InputData[3],6);} else {SETBIT(InputData[3],6);};
        // Test button
        tmp1 = GETBIT(Input[1],5); SETBIT(Input[1],5);
        if(!tmp1) {CLRBIT(InputData[1],1);} else {SETBIT(InputData[1],1);};
        if(!tmp1) {CLRBIT(InputData[3],1);} else {SETBIT(InputData[3],1);};
        // CoinX button
        tmp1 = GETBIT(Input[0],7); SETBIT(Input[0],7);
        if(!tmp1) {CLRBIT(InputData[1],2);} else {SETBIT(InputData[1],2);};
        tmp1 = GETBIT(Input[1],7); SETBIT(Input[1],7);
        if(!tmp1) {CLRBIT(InputData[3],2);} else {SETBIT(InputData[3],2);};
        InputData[0] = Input[0];                                               //    Andamiro uses unsigned short here also
        InputData[2] = Input[1];
    }

    return 0;
}


