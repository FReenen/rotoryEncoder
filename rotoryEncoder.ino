#define SENS0_PIN 2
#define SENS1_PIN 3
#define ANALOGOUT_PIN 11
#define HOMESW_PIN 10

#define DEBUG
#define DEBUG_CW
#define DEBUG_A

#define NUM_OF_TEATH 360*2                  // number of teath times 2
#define MAX_RPS 1                           // max rotation per second
#define DEG_STAP 3600/NUM_OF_TEATH          // degrees per step time 10
#define MIN_STAP_TIME MAX_RPS/NUM_OF_TEATH  // minimum time between staps in ms

uint32_t lastStap = 0; // last time of a stap
int16_t angle = 1800;     // curent angle times 10

/** setPWM
 * set the pwm dutycyle
 * 
 * @arg value (uint8_t): value for PWM; between 0 - 255
 */
void setPWM(uint8_t value){
  OCR2A = value;
}

void sens0(){
  if(digitalRead(SENS0_PIN) != digitalRead(SENS1_PIN)
     && millis() > lastStap + MIN_STAP_TIME){
    angle += DEG_STAP;
    lastStap = millis();
    #ifdef DEBUG_CW
    Serial.print("\t+");
    Serial.println(angle);
//  }else{
//    Serial.println("CCW2");
    #endif
  }
  
}
void sens1(){
  if(digitalRead(SENS0_PIN) != digitalRead(SENS1_PIN)
     && millis() > lastStap + MIN_STAP_TIME){
    angle -= DEG_STAP;
    lastStap = millis();
    #ifdef DEBUG_CW
    Serial.print("\t-");
    Serial.println(angle);
//  }else{
//    Serial.println("CW2");
    #endif
  }
}

void setup(){
  pinMode(HOMESW_PIN, INPUT_PULLUP);

  // setup analog out
  pinMode(ANALOGOUT_PIN, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  setPWM(0);

  // setup rotory sensor
  pinMode(SENS0_PIN, INPUT);
  pinMode(SENS1_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENS0_PIN), sens0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SENS1_PIN), sens1, CHANGE);
  
  #ifdef DEBUG
  Serial.begin(115200);

  Serial.println("rotoryEncoder by MReenen");
  Serial.print("deg per step: ");
  Serial.println(DEG_STAP, DEC);
  #endif
}

void loop(){
  // delay while checking the home sw
  for(uint16_t i = 65000; i != 0; i--){
    if(digitalRead(HOMESW_PIN) == LOW){
      angle = 0;
    }
  }

  // wrap angle around
  if(angle > 3600)
    angle -= 3600;
  else if(angle < 0)
    angle += 3600;

  #ifdef DEBUG_A
  Serial.println(angle);
  #endif

  // update PWM
  uint16_t a = angle;
  setPWM(map(a, 0, 3600, 0, 255));
}
