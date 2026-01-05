/*
 * Modified version of ArduinoPIUAux that adds in cab/pad lighting support
 * Configured to output lighting data to 3x 74xx595 shift registers hooked up to some MOSFETs to control lighting. You could use just 2 shift registers if you modified the code.
 * 
 * You need to upload two sketches to your Arduino to turn it into a PIUIO
 * The main code (either this one or ArduinoPIUAux) is uploaded normally to the Arduino
 * The USB code is compiled manually and uploaded to the serial processor via the Uno's DFU mode
 * See readme.md for more information
 */

#define GETBIT(port,bit) ((unsigned char)(((unsigned char)(port)) & ((unsigned char)(0x01 << ((unsigned char)(bit))))))
  #define SETBIT(port,bit) port |= (0x01 << (bit))    // RACERXL:   Set Byte bit
  #define CLRBIT(port,bit) port &= ~(0x01 << (bit))   // RACERXL:   Clr Byte bit

  static unsigned char Input[2];          // RACERXL:   The actual 16 bits Input data
  static unsigned char Output[3];         // RACERXL:   The actual 16 bits Output data

#define pinPad0 5 //P1 DR
#define pinPad1 3 //P1 DL
#define pinPad2 4 //P1 C
#define pinPad3 6 //P1 UR
#define pinPad4 2 //P1 UL
#define pinPad5 7 //P2 DR?
#define pinPad6 8 //P2 DL?
#define pinPad7 9 //P2 C
#define pinPad8 10 //P2 UR?
#define pinPad9 11 //P2 UL?

#define pinClear A0
#define pinService A2
#define pinTest A1
#define pinCoin 12

#define pinLEDDat A3 //Pins for the shift registers for lighting
#define pinLEDLat A4
#define pinLEDClk A5

bool enableManualLights = false; //Set to true to turn on panel lights when they're stepped on - disables the game controlling lighting

byte cabLEDs = 255;
byte padLEDsP1 = 0;
byte padLEDsP2 = 0;

static int prev_switch;
static int prev_switch_state;

void setup() {
    pinMode(pinClear, INPUT_PULLUP); //Clear
    pinMode(pinService, INPUT_PULLUP); //Service
    pinMode(pinTest, INPUT_PULLUP); //Test
    
    pinMode(pinPad0, INPUT);// Pad start
    pinMode(pinPad1, INPUT);
    pinMode(pinPad2, INPUT);
    pinMode(pinPad3, INPUT);
    pinMode(pinPad4, INPUT);
    pinMode(pinPad5, INPUT);
    pinMode(pinPad6, INPUT);
    pinMode(pinPad7, INPUT);
    pinMode(pinPad8, INPUT);
    pinMode(pinPad9, INPUT);// Pad end
    
    pinMode(pinCoin, INPUT_PULLUP);//Coin

    pinMode(pinLEDDat, OUTPUT); //Lighting shift register pins
    pinMode(pinLEDLat, OUTPUT);
    pinMode(pinLEDClk, OUTPUT);
    pinMode(13, INPUT_PULLUP); // For the switch

    Serial.begin(38400);
  Input[0]=0xFF;
  Input[1]=0xFF;

  cabLEDs = 255; padLEDsP1 = 0; padLEDsP2 = 0; //Write initial data to the shift registers for lighting
  writeLighting();

  prev_switch = digitalRead(13)?1:0;
  for(int i = 1; i < 4; i++) {
      prev_switch |= (prev_switch & 1) << i;
  }
  prev_switch_state = prev_switch == 0xF?1:0;
}

unsigned char inputn,tmp1; 
#define SET_THING if(tmp1) SETBIT(Input[inputn/8],inputn%8); else CLRBIT(Input[inputn/8],inputn%8);
void loop() {
POINT_RETURN:
  while (Serial.available() > 0) {
    byte inByte = Serial.read();
    if(inByte == 0x56)
    {
      Serial.write(0xAE);
      Serial.flush();
      for(int i = 0; i < 3; i++)
      {
        int count = 0;  
        while (Serial.available() <= 0)
        {
          count++;
          if(count >= 10000) goto POINT_RETURN;
        }
        Output[i] = Serial.read();
      }
      for(int i = 0; i < 2; i++)
      {
        Serial.write(Input[i]);
        Serial.flush(); 
  //Serial.print(Input[1], BIN);
  //Serial.println(Input[0], BIN);
      }
    }    
  }

  if (!enableManualLights) { //Using game-controlled lighting, read lighting data and set lighting variables
    cabLEDs = 255; padLEDsP1 = 0; padLEDsP2 = 0;
    //padLEDsP1 = Output[0]; padLEDsP2 = Output[1]; cabLEDs = Output[2];
    //Supposedly this is what the format is (from lowest to highest bit):
    //Source: https://github.com/racerxdl/piuio_clone/blob/master/docs/piuio.txt , modified with my own findings
    //Byte0: ZZABCDEx
    //Byte1: ZZABCDEI
    //Byte2: HGFxxxHN
    //A = UL, B = UR, C = C, D = DL, E = DR
    //N = Neon (in a byte not broken out?)
    //FG = Marquee L1/2
    //HI = Marquee R1/2

    if (bitRead(Output[0], 2)) { bitSet(padLEDsP1, 4); } //P1 Pad: UL
    if (bitRead(Output[0], 3)) { bitSet(padLEDsP1, 0); } //UR
    if (bitRead(Output[0], 4)) { bitSet(padLEDsP1, 2); } //C
    if (bitRead(Output[0], 5)) { bitSet(padLEDsP1, 3); } //DL
    if (bitRead(Output[0], 6)) { bitSet(padLEDsP1, 1); } //DR
    
    if (bitRead(Output[1], 2)) { bitSet(padLEDsP2, 4); } //P2 Pad: UL
    if (bitRead(Output[1], 3)) { bitSet(padLEDsP2, 0); } //UR
    if (bitRead(Output[1], 4)) { bitSet(padLEDsP2, 2); } //C
    if (bitRead(Output[1], 5)) { bitSet(padLEDsP2, 3); } //DL
    if (bitRead(Output[1], 6)) { bitSet(padLEDsP2, 1); } //DR
    
    if (bitRead(Output[2], 0)) { bitClear(cabLEDs, 1); } //Cab lights: Marquee R1. Inverted for use with the amp box
    if (bitRead(Output[2], 1)) { bitClear(cabLEDs, 3); } //Marquee R2
    if (bitRead(Output[1], 7)) { bitClear(cabLEDs, 2); } //Marquee L2
    if (bitRead(Output[2], 2)) { bitClear(cabLEDs, 4); } //Marquee L1
    if (bitRead(Output[2], 7)) { bitClear(cabLEDs, 5); } //Bass neon

    
  }

  if (enableManualLights) {cabLEDs = 255; padLEDsP1 = 0; padLEDsP2 = 0;} //Set initial lighting data if using manual lighting
  inputn = 4;   tmp1 = digitalRead(pinPad0); if (enableManualLights && tmp1) { bitSet(padLEDsP1, 4); } SET_THING //P1 DR
  inputn = 3;   tmp1 = digitalRead(pinPad1); if (enableManualLights && tmp1) { bitSet(padLEDsP1, 0); } SET_THING //P1 DL
  inputn = 2;   tmp1 = digitalRead(pinPad2); if (enableManualLights && tmp1) { bitSet(padLEDsP1, 3); } SET_THING //P1 C
  inputn = 1;   tmp1 = digitalRead(pinPad3); if (enableManualLights && tmp1) { bitSet(padLEDsP1, 1); } SET_THING //P1 UR
  inputn = 0;   tmp1 = digitalRead(pinPad4); if (enableManualLights && tmp1) { bitSet(padLEDsP1, 2); } SET_THING //P1 UL
  inputn = 12;  tmp1 = digitalRead(pinPad5); if (enableManualLights && tmp1) { bitSet(padLEDsP2, 4); } SET_THING //P2 
  inputn = 11;  tmp1 = digitalRead(pinPad6); if (enableManualLights && tmp1) { bitSet(padLEDsP2, 0); } SET_THING //P2 
  inputn = 10;  tmp1 = digitalRead(pinPad7); if (enableManualLights && tmp1) { bitSet(padLEDsP2, 3); } SET_THING //P2 C
  inputn = 9;   tmp1 = digitalRead(pinPad8); if (enableManualLights && tmp1) { bitSet(padLEDsP2, 1); } SET_THING //P2 
  inputn = 8;   tmp1 = digitalRead(pinPad9); if (enableManualLights && tmp1) { bitSet(padLEDsP2, 2); } SET_THING //P2 
  inputn = 5;   tmp1 = digitalRead(pinClear); SET_THING // CLEAR?
  inputn = 6;   tmp1 = digitalRead(pinService); SET_THING // SERVICE
  inputn = 8+5; tmp1 = digitalRead(pinTest); SET_THING // TEST
  inputn = 7;   tmp1 = digitalRead(pinCoin); SET_THING // COIN 1
  //inputn = 8+7; tmp1 = digitalRead(A5)?0:1; SET_THING // COIN 2 ?

  
  writeLighting();

  // This has a debouncer
  prev_switch = prev_switch << 1;
  prev_switch |= digitalRead(13)?1:0;
  prev_switch &= 0xF;
  int cur_switch_state = prev_switch_state;
  if(prev_switch == 0xF) cur_switch_state = 1;
  else if(prev_switch == 0x0) cur_switch_state = 0;
  if(prev_switch_state != cur_switch_state && !cur_switch_state) {
    Serial.write(0xDF);
    Serial.flush();
  }
  prev_switch_state = cur_switch_state;
}

//Writes lighting data to the 3 shift registers - Code can be modified to only use 2
void writeLighting() {
  digitalWrite(pinLEDLat, LOW); //Set latch pin low and start shifting out the data
  shiftOut(pinLEDDat, pinLEDClk, LSBFIRST, padLEDsP2);
  shiftOut(pinLEDDat, pinLEDClk, LSBFIRST, cabLEDs);
  shiftOut(pinLEDDat, pinLEDClk, LSBFIRST, padLEDsP1);
  digitalWrite(pinLEDLat, HIGH); //Set latch pin high to make the shift registers output the new data
}
