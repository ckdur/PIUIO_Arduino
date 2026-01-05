/*
 * ArduinoPIUAux - Push-pull logic in the arduino side (ATMEGA side)
 */

#define GETBIT(port,bit) ((unsigned char)(((unsigned char)(port)) & ((unsigned char)(0x01 << ((unsigned char)(bit))))))
#define SETBIT(port,bit) port |= (0x01 << (bit))
#define CLRBIT(port,bit) port &= ~(0x01 << (bit))

static unsigned char Input[2];
static unsigned char Output[3];

static int prev_switch;
static int prev_switch_state;

void setup() {
  pinMode(A0, INPUT_PULLUP);
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

  prev_switch = digitalRead(A0)?1:0;
  for(int i = 1; i < 4; i++) {
      prev_switch |= (prev_switch & 1) << i;
  }
  prev_switch_state = prev_switch == 0xF?1:0;
}

#define SET_THING(a, ind) if(a) SETBIT(Input[ind/8],ind%8); else CLRBIT(Input[ind/8],ind%8);
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
        // Serial.print(Input[1], BIN);
        // Serial.println(Input[0], BIN);
      }
    }    
  }

  SET_THING(digitalRead(2), 4);
  SET_THING(digitalRead(3), 3);
  SET_THING(digitalRead(4), 2);
  SET_THING(digitalRead(5), 1);
  SET_THING(digitalRead(6), 0);
  SET_THING(digitalRead(7), 12);
  SET_THING(digitalRead(8), 11);
  SET_THING(digitalRead(9), 10);
  SET_THING(digitalRead(10), 9);
  SET_THING(digitalRead(11), 8);
  // SET_THING(digitalRead(A1), 5);   // CLEAR
  SET_THING(digitalRead(12), 6);      // SERVICE
  SET_THING(digitalRead(13), 13);     // TEST
  // SET_THING(digitalRead(A4), 7);   // COIN 1
  // SET_THING(digitalRead(A5), 15);  // COIN 2

  // This has a debouncer
  prev_switch = prev_switch << 1;
  prev_switch |= digitalRead(A0)?1:0;
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
