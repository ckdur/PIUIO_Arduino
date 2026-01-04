/***********************************************************/
/*   ____ ___ _   _ ___ ___     ____ _                     */
/*  |  _ \_ _| | | |_ _/ _ \   / ___| | ___  _ __   ___    */
/*  | |_) | || | | || | | | | | |   | |/ _ \| '_ \ / _ \   */
/*  |  __/| || |_| || | |_| | | |___| | (_) | | | |  __/   */
/*  |_|  |___|\___/|___\___/   \____|_|\___/|_| |_|\___|   */
/*                                                         */
/*  By: USB-ON-ATMEGAU4 BY CKDUR                           */
/***********************************************************/
/*    Basicly this is an PIUIO Clone with an ATMEGA32U2    */
/*            interfaced on ARDUINO Leonardo               */
/***********************************************************/
/*          This is main code from PIUIO Clone             */
/***********************************************************/
/*          Please look at LICENSE for details             */
/*  Please consult https://github.com/ckdur/PIUIO_arduino  */
/***********************************************************/

#include "PIUIO.h"
#include "PIUIO_ctrl.h"

#define PIUIO_CTL_REQ 0xAE

#define GETBIT(port,bit) ((unsigned char)(((unsigned char)(port)) & ((unsigned char)(0x01 << ((unsigned char)(bit))))))
#define SETBIT(port,bit) port |= (0x01 << (bit))
#define CLRBIT(port,bit) port &= ~(0x01 << (bit))

// This turns on one of the LEDs hooked up to the chip
// NOTE: use the enum for the port
enum IO_PORTS {
    IO_PORT_B,
    IO_PORT_C,
    IO_PORT_D,
    IO_PORT_E,
    IO_PORT_F
};

// Sets or unsets the output mode
void MODEon(char ledNumber, int port) {
    if(port == IO_PORT_B) {
        DDRB |= 1 << ledNumber;
    }
    if(port == IO_PORT_C) {
        DDRC |= 1 << ledNumber;
    }
    if(port == IO_PORT_D) {
        DDRD |= 1 << ledNumber;
    }
    if(port == IO_PORT_E) {
        DDRE |= 1 << ledNumber;
    }
    if(port == IO_PORT_F) {
        DDRF |= 1 << ledNumber;
    }
}

void MODEoff(char ledNumber, int port) {
    if(port == IO_PORT_B) {
        DDRB &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_C) {
        DDRC &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_D) {
        DDRD &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_E) {
        DDRE &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_F) {
        DDRF &= ~(1 << ledNumber);
    }
}

// Sets or unsets the output (remember to set the mode first)
void LEDon(char ledNumber, int port){
    if(port == IO_PORT_B) {
        PORTB |= 1 << ledNumber;
    }
    if(port == IO_PORT_C) {
        PORTC |= 1 << ledNumber;
    }
    if(port == IO_PORT_D) {
        PORTD |= 1 << ledNumber;
    }
    if(port == IO_PORT_E) {
        PORTE |= 1 << ledNumber;
    }
    if(port == IO_PORT_F) {
        PORTF |= 1 << ledNumber;
    }
}

// And this turns it off
void LEDoff(char ledNumber, int port){
    if(port == IO_PORT_B) {
        PORTB &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_C) {
        PORTC &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_D) {
        PORTD &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_E) {
        PORTE &= ~(1 << ledNumber);
    }
    if(port == IO_PORT_F) {
        PORTF &= ~(1 << ledNumber);
    }
}

int READfrom(char ledNumber, int port){
    int st = 0;
    if(port == IO_PORT_B) {
        st = (PINB & (1 << ledNumber)) ? 1 : 0;
    }
    if(port == IO_PORT_C) {
        st = (PINC & (1 << ledNumber)) ? 1 : 0;
    }
    if(port == IO_PORT_D) {
        st = (PIND & (1 << ledNumber)) ? 1 : 0;
    }
    if(port == IO_PORT_E) {
        st = (PINE & (1 << ledNumber)) ? 1 : 0;
    }
    if(port == IO_PORT_F) {
        st = (PINF & (1 << ledNumber)) ? 1 : 0;
    }
    return st;
}

// Configurations of all
#define RXLED 0, IO_PORT_B
#define TXLED 5, IO_PORT_D

// As for the rest...
// Check https://www.arduino.cc/en/uploads/Main/arduino-leonardo-schematic_3b.pdf for details
#define DIO0 PD2, IO_PORT_D
#define DIO1 PD3, IO_PORT_D
#define DIO2 PD1, IO_PORT_D
#define DIO3 PD0, IO_PORT_D
#define DIO4 PD4, IO_PORT_D
#define DIO5 PC6, IO_PORT_C
#define DIO6 PD7, IO_PORT_D
#define DIO7 PE6, IO_PORT_E
#define DIO8 PB4, IO_PORT_B
#define DIO9 PB5, IO_PORT_B
#define DIO10 PB6, IO_PORT_B
#define DIO11 PB7, IO_PORT_B
#define DIO12 PD6, IO_PORT_D
#define DIO13 PC7, IO_PORT_C
#define DIOA0 PF7, IO_PORT_F
#define DIOA1 PF6, IO_PORT_F
#define DIOA2 PF5, IO_PORT_F
#define DIOA3 PF4, IO_PORT_F
#define DIOA4 PF1, IO_PORT_F
#define DIOA5 PF0, IO_PORT_F

// Now, assigning the IO according to ColombiaStep's stuff

#define P1_PAD
#define P2_PAD
//#define DO_LED

#if defined(P1_PAD) && defined(P2_PAD) && defined(DO_LED)
#error "This implementation does not support P1 & P2 and Lights. You need to disable one of them"
#endif

#ifdef P1_PAD
    #define KEY_Q DIO8
    #define KEY_E DIO4
    #define KEY_S DIO10
    #define KEY_Z DIO6
    #define KEY_C DIO2

    #ifdef DO_LED
        #define LED_Q DIO7
        #define LED_E DIO3
        #define LED_S DIO9
        #define LED_Z DIO5
        #define LED_C DIO1
    #endif // DO_LED
#endif

#ifdef P2_PAD
    #ifndef P1_PAD
        #define KEY_7 DIO8
        #define KEY_9 DIO4
        #define KEY_5 DIO10
        #define KEY_1 DIO6
        #define KEY_3 DIO2
    #else
        #define KEY_7 DIO7
        #define KEY_9 DIO3
        #define KEY_5 DIO9
        #define KEY_1 DIO5
        #define KEY_3 DIO1
    #endif

    #ifdef DO_LED
        #define LED_7 DIO7
        #define LED_9 DIO3
        #define LED_5 DIO9
        #define LED_1 DIO5
        #define LED_3 DIO1
    #endif // DO_LED
#endif

#define KEY_COIN1 DIO13
//#define KEY_COIN2 DIO13
//#define KEY_CLEAR DIO13
#define KEY_SERVICE DIO12
#define KEY_TEST DIO11

#define LED_NEON DIOA0
#define LED_UL DIOA1
#define LED_DL DIOA2
#define LED_UR DIOA3
#define LED_DL DIOA4

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    nControl = 0;
    SetupHardware();

    GlobalInterruptEnable();

    int i;
    for(i=0;i<8;i++){
        InputData[i] = 0xFF;
        LampData[i] = 0x0;

    }

#ifdef KEY_Q
    MODEoff(KEY_Q); LEDon(KEY_Q);
#endif
#ifdef KEY_E
    MODEoff(KEY_E); LEDon(KEY_E);
#endif
#ifdef KEY_S
    MODEoff(KEY_S); LEDon(KEY_S);
#endif
#ifdef KEY_Z
    MODEoff(KEY_Z); LEDon(KEY_Z);
#endif
#ifdef KEY_C
    MODEoff(KEY_C); LEDon(KEY_C);
#endif
#ifdef KEY_7
    MODEoff(KEY_7); LEDon(KEY_7);
#endif
#ifdef KEY_9
    MODEoff(KEY_9); LEDon(KEY_9);
#endif
#ifdef KEY_5
    MODEoff(KEY_5); LEDon(KEY_5);
#endif
#ifdef KEY_1
    MODEoff(KEY_1); LEDon(KEY_1);
#endif
#ifdef KEY_3
    MODEoff(KEY_3); LEDon(KEY_3);
#endif
#ifdef KEY_CLEAR
    MODEoff(KEY_CLEAR); LEDon(KEY_CLEAR);
#endif
#ifdef KEY_SERVICE
    MODEoff(KEY_SERVICE); LEDon(KEY_SERVICE);
#endif
#ifdef KEY_TEST
    MODEoff(KEY_TEST); LEDon(KEY_TEST);
#endif
#ifdef KEY_COIN1
    MODEoff(KEY_COIN1); LEDon(KEY_COIN1);
#endif
#ifdef KEY_COIN2
    MODEoff(KEY_COIN2); LEDon(KEY_COIN2);
#endif

#ifdef LED_Q
    MODEon(LED_Q);
#endif
#ifdef LED_E
    MODEon(LED_E);
#endif
#ifdef LED_S
    MODEon(LED_S);
#endif
#ifdef LED_Z
    MODEon(LED_Z);
#endif
#ifdef LED_C
    MODEon(LED_C);
#endif
#ifdef LED_7
    MODEon(LED_7);
#endif
#ifdef LED_9
    MODEon(LED_9);
#endif
#ifdef LED_5
    MODEon(LED_5);
#endif
#ifdef LED_1
    MODEon(LED_1);
#endif
#ifdef LED_3
    MODEon(LED_3);
#endif
#ifdef LED_NEON
    MODEon(LED_NEON);
#endif
#ifdef LED_UL
    MODEon(LED_UL);
#endif
#ifdef LED_DL
    MODEon(LED_DL);
#endif
#ifdef LED_UR
    MODEon(LED_UR);
#endif
#ifdef LED_DR
    MODEon(LED_DR);
#endif

    for (;;)
    {
RESTART:
        USB_Loop();
        USB_USBTask();
        
        if(nControl > 1000) LEDon(RXLED);
        if(nControl > 2000) {LEDoff(RXLED); nControl = 0;}
        
#ifdef KEY_Q
        if(READfrom(KEY_Q)) {SETBIT(InputData[0],0);} else {CLRBIT(InputData[0],0);}
#endif
#ifdef KEY_E
        if(READfrom(KEY_E)) {SETBIT(InputData[0],1);} else {CLRBIT(InputData[0],1);}
#endif
#ifdef KEY_S
        if(READfrom(KEY_S)) {SETBIT(InputData[0],2);} else {CLRBIT(InputData[0],2);}
#endif
#ifdef KEY_Z
        if(READfrom(KEY_Z)) {SETBIT(InputData[0],3);} else {CLRBIT(InputData[0],3);}
#endif
#ifdef KEY_C
        if(READfrom(KEY_C)) {SETBIT(InputData[0],4);} else {CLRBIT(InputData[0],4);}
#endif
#ifdef KEY_7
        if(READfrom(KEY_7)) {SETBIT(InputData[2],0);} else {CLRBIT(InputData[2],0);}
#endif
#ifdef KEY_9
        if(READfrom(KEY_9)) {SETBIT(InputData[2],1);} else {CLRBIT(InputData[2],1);}
#endif
#ifdef KEY_5
        if(READfrom(KEY_5)) {SETBIT(InputData[2],2);} else {CLRBIT(InputData[2],2);}
#endif
#ifdef KEY_1
        if(READfrom(KEY_1)) {SETBIT(InputData[2],3);} else {CLRBIT(InputData[2],3);}
#endif
#ifdef KEY_3
        if(READfrom(KEY_3)) {SETBIT(InputData[2],4);} else {CLRBIT(InputData[2],4);}
#endif
#ifdef KEY_CLEAR
        if(READfrom(KEY_CLEAR)) {SETBIT(InputData[1],7);} else {CLRBIT(InputData[1],7);}
        if(READfrom(KEY_CLEAR)) {SETBIT(InputData[3],7);} else {CLRBIT(InputData[3],7);}
#endif
#ifdef KEY_SERVICE
        if(READfrom(KEY_SERVICE))  {SETBIT(InputData[1],6);} else {CLRBIT(InputData[1],6);}
        if(READfrom(KEY_SERVICE))  {SETBIT(InputData[3],6);} else {CLRBIT(InputData[3],6);}
#endif
#ifdef KEY_TEST
        if(READfrom(KEY_TEST)) {SETBIT(InputData[1],1);} else {CLRBIT(InputData[1],1);}
        if(READfrom(KEY_TEST)) {SETBIT(InputData[3],1);} else {CLRBIT(InputData[3],1);}
#endif
#ifdef KEY_COIN1
        if(READfrom(KEY_COIN1)) {SETBIT(InputData[1],2);} else {CLRBIT(InputData[1],2);}
#endif
#ifdef KEY_COIN2
        if(READfrom(KEY_COIN2)) {SETBIT(InputData[3],2);} else {CLRBIT(InputData[3],2);}
#endif

#ifdef LED_Q
        if(GETBIT(LampData[0],2)) {LEDon(LED_Q);} else {LEDoff(LED_Q);}
#endif
#ifdef LED_E
        if(GETBIT(LampData[0],3)) {LEDon(LED_E);} else {LEDoff(LED_E);}
#endif
#ifdef LED_S
        if(GETBIT(LampData[0],4)) {LEDon(LED_Q);} else {LEDoff(LED_Q);}
#endif
#ifdef LED_Z
        if(GETBIT(LampData[0],5)) {LEDon(LED_Z);} else {LEDoff(LED_Z);}
#endif
#ifdef LED_C
        if(GETBIT(LampData[0],6)) {LEDon(LED_C);} else {LEDoff(LED_C);}
#endif
#ifdef LED_7
        if(GETBIT(LampData[2],2)) {LEDon(LED_7);} else {LEDoff(LED_7);}
#endif
#ifdef LED_9
        if(GETBIT(LampData[2],3)) {LEDon(LED_9);} else {LEDoff(LED_9);}
#endif
#ifdef LED_5
        if(GETBIT(LampData[2],4)) {LEDon(LED_5);} else {LEDoff(LED_5);}
#endif
#ifdef LED_1
        if(GETBIT(LampData[2],5)) {LEDon(LED_1);} else {LEDoff(LED_1);}
#endif
#ifdef LED_3
        if(GETBIT(LampData[2],6)) {LEDon(LED_3);} else {LEDoff(LED_3);}
#endif
#ifdef LED_NEON
        if(GETBIT(LampData[1],2)) {LEDon(LED_NEON);} else {LEDoff(LED_NEON);}
#endif
#ifdef LED_UL
        if(GETBIT(LampData[2],7)) {LEDon(LED_UL);} else {LEDoff(LED_UL);}
#endif
#ifdef LED_DL
        if(GETBIT(LampData[3],0)) {LEDon(LED_DL);} else {LEDoff(LED_DL);}
#endif
#ifdef LED_UR
        if(GETBIT(LampData[3],1)) {LEDon(LED_UR);} else {LEDoff(LED_UR);}
#endif
#ifdef LED_DR
        if(GETBIT(LampData[3],2)) {LEDon(LED_DR);} else {LEDoff(LED_DR);}
#endif
        
        // Note: Logic is inverted in these leds
        if(InputData[0] != 0xFF || InputData[1] != 0xFF || InputData[2] != 0xFF || InputData[3] != 0xFF) LEDoff(TXLED); else LEDon(TXLED);
    }

    return 0;
}
