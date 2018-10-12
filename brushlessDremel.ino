#include <Servo.h>

//run the pump at no less than 50%
#define MINIMUM_COOLER_FACTOR 200

int escPin=9;
int switchPin=8;
int potPin=1;
int coolerPin=10;

int val;

Servo esc;

void setup() {
  esc.attach(escPin);
  pinMode(switchPin, INPUT);
  pinMode(coolerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //if(digitalRead(switchPin)){
    val = analogRead(potPin);
    if (val > MINIMUM_COOLER_FACTOR) {
      digitalWrite(coolerPin, 1);
    } else {
      digitalWrite(coolerPin, 0);
    }
    val = map(val, 0, 1023, 1000, 2000);
    esc.writeMicroseconds(val);
  //} else { //failsafe mode - externally pulled low
  //  esc.write(0);
  //}
}
