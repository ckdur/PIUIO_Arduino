#define GETBIT(port,bit) ((unsigned char)(((unsigned char)(port)) & ((unsigned char)(0x01 << ((unsigned char)(bit))))))
  #define SETBIT(port,bit) port |= (0x01 << (bit))    // RACERXL:   Set Byte bit
  #define CLRBIT(port,bit) port &= ~(0x01 << (bit))   // RACERXL:   Clr Byte bit

  static unsigned char Input[2];          // RACERXL:   The actual 16 bits Input data
  static unsigned char Output[3];         // RACERXL:   The actual 16 bits Output data

void setup() {
  pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
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

    Serial.begin(38400);
  Input[0]=0xFF;
  Input[1]=0xFF;
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
  //inputn = 5; tmp1 = digitalRead(A1); SET_THING // CLEAR
  inputn = 6; tmp1 = digitalRead(12); SET_THING // SERVICE
  inputn = 8+5; tmp1 = digitalRead(13); SET_THING // TEST
  //inputn = 7; tmp1 = digitalRead(A4); SET_THING // COIN 1 ?
  //inputn = 8+7; tmp1 = digitalRead(A5)?0:1; SET_THING // COIN 2 ?
}
