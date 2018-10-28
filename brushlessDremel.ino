#include <Servo.h>

//run the pump at no less than 50%
#define MINIMUM_COOLER_FACTOR 200

int escPin=9;
int switchPin=8;
int potPin=2;
int coolerPin=10;
bool motorRunning = false;

int val; //temporary to hold pre-mapped motor speed. Originally this was just mapped through but since we have an interrupt using the same variable we can't in all good knowledge use it.
int motorSpeed;
int coolerSpeed;

unsigned long accumulator = 0;

//timer interrupt to handle the accumulator for cooling
SIGNAL(TIMER0_COMPA_vect){
	if (motorRunning){ //do as little as possible in here - the actual motor setting is done in the main loop
    accumulator++;
	}
}

Servo esc;

//
void checkAndRunCooler(){
  if (accumulator > 0){
    coolerSpeed = (accumulator/60 > 255 ? accumulator/60 : 255); //can a compiler handle this? are there enough registers in the 168 to handle this? Who knows
    analogWrite(coolerPin, coolerSpeed);
    accumulator--;
    Serial.print("Cooler: ");
    Serial.println(coolerSpeed);
  } else {
    coolerSpeed = 0;
    digitalWrite(coolerPin, coolerSpeed);
    Serial.println(coolerSpeed);
  }
}

void checkAndRunMotor(int val){
  motorSpeed = map(val,0,1023,1000,2000);
  esc.writeMicroseconds(motorSpeed);
}

void setup() {
  //it doesn't actually matter how often we interrupt since the accumulator is unitless. Don't want to overflow it though, so we go as slow as we can (still faster than a second per interrupt)
  OCR0A = 0xFF;
  TIMSK0 = _BV(OCIE0A); //enable the interrupt
  esc.attach(escPin);
  pinMode(switchPin, INPUT);
  pinMode(coolerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //if(digitalRead(switchPin)){
  
    val = analogRead(potPin);
    if (val > MINIMUM_COOLER_FACTOR) { //run the cooler while the accumulator is still going
	    motorRunning = true;
    } else {
	    motorRunning = false;
    }
    
    checkAndRunMotor(val);
    checkAndRunCooler();
    Serial.println(accumulator);
  //} else { //failsafe mode - externally pulled low
  //  esc.write(0);
  //}
}
