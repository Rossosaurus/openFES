#include <Arduino.h>
#include <Wire.h>
#include <AD5252.h>

AD5252 digiPot(0x2C);
int C1_1 = 4;
int C1_2 = 5;
int C2_1 = 6;
int C2_2 = 7;
int min = 0;
int max = 250;
int value;

void setup() {
  pinMode(C1_1, OUTPUT);
  pinMode(C1_2, OUTPUT);
  pinMode(C2_1, OUTPUT);
  pinMode(C2_2, OUTPUT);
  digitalWrite(C1_1, HIGH);
  digitalWrite(C1_2, HIGH);
  digitalWrite(C2_1, HIGH);
  digitalWrite(C2_2, HIGH);

  Serial.begin(115200);
  Wire.begin();
  if (digiPot.begin()) {
    Serial.println("AD5252 found");
  } else {
    Serial.println("AD5252 not found");
    return;
  }
}

void loop() {

  if (Serial.available() > 0) {
    value = Serial.parseInt();
    if (value >= min && value <= max) {
      digiPot.write(0, value);
      digiPot.write(1, value);
      Serial.println(value);
    }
  }
  // for (int i = 0; i < 251; i = i + 10) {
  //   value = min + i;
  //   digiPot.write(0, value);
  //   digiPot.write(1, value);
  //   Serial.println(value);
  //   delay(250);
  // }
  // for (int i = 160; i > -1; i = i - 10) {
  //   value = min + i;
  //   digiPot.write(0, value);
  //   digiPot.write(1, value);
  //   Serial.println(value);
  //   delay(250);
  // }
}