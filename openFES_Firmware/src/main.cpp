#include <Arduino.h>
#include <Wire.h>
#include <AD5252.h>
#include <AltSoftSerial.h>
#include <EMSChannel.h>
#include <RN4020.h>

int C1_1 = 4;
int C1_2 = 5;
int C1_LED = A2;

int C2_1 = 6;
int C2_2 = 7;
int C2_LED = A3;

int wakePin = 8;

String command = "";

AltSoftSerial rn4020;
AD5252 digiPot;
RN4020 btRN4020(&rn4020, wakePin);

void activateChannel(int C, int I, long int D);
void processCommand(String cmd);

void setup() {
  pinMode(C1_1, OUTPUT);
  pinMode(C1_2, OUTPUT);
  pinMode(C1_LED, OUTPUT);
  pinMode(C2_1, OUTPUT);
  pinMode(C2_2, OUTPUT);
  pinMode(C2_LED, OUTPUT);
  pinMode(wakePin, OUTPUT);
  // Start Serial communication with PC and bluetooth module
  Serial.begin(115200);
  while(!Serial);
  btRN4020.wake();
  btRN4020.init(); 

  // Start I2C communication with digiPot
  Wire.begin();
  if (digiPot.begin()) {
    Serial.println("AD5252 found");
  } else {
    Serial.println("AD5252 not found");
    return;
  }

  Serial.println("Setup complete...");
}

void loop() {
  if (rn4020.available() > 0) {
    command = rn4020.readStringUntil('\n');
    command.trim();
    Serial.println("Command recieved: " + command);
    processCommand(command);
  }
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println("Command recieved: " + command);
    processCommand(command);
  }
}

void processCommand(String cmd) {
  int channel = 0;
  int intensity = 0;
  int duration = 0;
  Serial.println("Processing command: " + cmd);
  if (String(cmd[0]) == "S") {
    channel = cmd.substring(2, 3).toInt() - 1;
    intensity = cmd.substring(4,7).toInt();
    duration = cmd.substring(8).toInt();
    Serial.println("Signal Command...");
    Serial.println("Channel: " + String(channel));
    Serial.println("Intensity: " + String(intensity));
    Serial.println("Duration: " + String(duration));
    activateChannel(channel, intensity, duration);
  } else if (String(cmd[0]) == "B") {
    cmd = cmd.substring(1);
    if (cmd == "PAIR" || cmd == "pair") {
      btRN4020.pair();
    }
    else if (cmd == "UNPAIR" || cmd == "unpair") {
      btRN4020.unpair();
    }
    else if (cmd.substring(0,4) == "NAME" || cmd.substring(0,4) == "NAME") {
      btRN4020.setName(cmd.substring(4));
    }
    else if (cmd == "TALK" || cmd == "talk") {
      btRN4020.talk();
    }
  } else {
    Serial.println("Command not recognised");
  }
}

void activateChannel(int C, int I, long int D) {
  Serial.println("Activating channel: " + String(C) + " with intensity: " + String(I) + " for duration: " + String(D));
  if (C == 0) {
    digitalWrite(C1_1, HIGH);
    digitalWrite(C1_2, HIGH);
    digitalWrite(C1_LED, HIGH);
  } else if (C == 1) {
    digitalWrite(C2_1, HIGH);
    digitalWrite(C2_2, HIGH);
    analogWrite(C2_LED, HIGH);
  }
  else {
    Serial.println("Invalid channel");
    return;
  }

  I = floor((I/100) * 255);
  digiPot.write(C, I);
  
  delay(D);
  digitalWrite(C1_1, LOW);
  digitalWrite(C1_2, LOW);
  digitalWrite(C1_LED, LOW);
  digitalWrite(C2_1, LOW);
  digitalWrite(C2_2, LOW);
  digitalWrite(C2_LED, LOW);
}