#define SENS0_PIN 2
#define SENS1_PIN 3
#define ANALOGOUT_PIN 11
#define HOMESW_PIN 10

#define NUMOFTEATH 360
#define DEGPERSTAP 3600/(NUMOFTEATH*2)

int16_t angle = 0;

/** setPWM
 * set the pwm dutycyle
 * 
 * @arg value (uint8_t): value for PWM; between 0 - 255
 */
void setPWM(uint8_t value){
  OCR2A = value;
}

void sens0(){
  if(digitalRead(SENS0_PIN) != digitalRead(SENS1_PIN)){
    angle += DEGPERSTAP * 10;
  }
}
void sens1(){
  if(digitalRead(SENS0_PIN) != digitalRead(SENS1_PIN)){
    angle -= DEGPERSTAP * 10;
  }
}

void setup(){
  pinMode(HOMESW_PIN, INPUT_PULLUP);

  // setup analog out
  pinMode(ANALOGOUT_PIN, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  setPWM(0);

  // setup rotory sensor
  pinMode(SENS0_PIN, INPUT);
  pinMode(SENS1_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENS0_PIN), sens0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SENS1_PIN), sens1, CHANGE);
}

void loop(){
  // delay while checking the home sw
  for(uint8_t i = 200; i != 0; i--){
    if(digitalRead(HOMESW_PIN) == LOW){
      angle = 0;
    }
  }

  // wrap angle around
  if(angle > 3600)
    angle -= 3600;
  else if(angle < 0)
    angle += 3600;

  // update PWM
  uint16_t a = angle;
  setPWM(map(a, 0, 360, 0, 255));
}
