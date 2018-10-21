#include <Servo.h>

//run the pump at no less than 50%
#define MINIMUM_COOLER_FACTOR 200

int escPin=9;
int switchPin=8;
int potPin=1;
int coolerPin=10;
bool motorRunning = true;

int val;

int accumulator = 0;

//timer interrupt to handle the accumulator for cooling
SIGNAL(TIMER0_COMPA_vect){
	if (accumulator){
		digitalWrite(coolerPin, 1);
		if (!motorRunning) accumulator--;
	} else {
		digitalWrite(coolerPin, 0);
	}
	if (motorRunning){
    accumulator++;
	}
}

Servo esc;

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
    val = map(val, 0, 1023, 1000, 2000);
    esc.writeMicroseconds(val);
//    Serial.println(accumulator);
  //} else { //failsafe mode - externally pulled low
  //  esc.write(0);
  //}
}
