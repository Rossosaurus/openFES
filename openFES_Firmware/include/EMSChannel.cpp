/*
 *  EMSChannel.h
 * 
 *  Created on: 20/02/2025
 *  Author: 	Ross Underhill
 * 
 *  Description: Class controlling EMS stimulation channels
 */

#include "EMSChannel.h"

EMSChannel::EMSChannel(int switch1, int switch2, int channelLED, AD5252 *digiPot, int wiperIndex) {
    this->sw1 = switch1;
    this->sw2 = switch2;
    this->LED = channelLED;
    this->pot = digiPot;
    this->wiper = wiperIndex;

    pinMode(sw1, OUTPUT);
    pinMode(sw2, OUTPUT);
    pinMode(LED, OUTPUT);

    digitalWrite(sw1, LOW);
    digitalWrite(sw2, LOW);
    digitalWrite(LED, LOW);

    Wire.begin();
    if (pot->begin()) {
        Serial.println("AD5252 connected");
    } else {
        Serial.println("No connection to AD5252 found");
        while (true) {delay(2000);}
    }
}

EMSChannel::~EMSChannel() {
    digitalWrite(sw1, LOW);
    digitalWrite(sw2, LOW);
    digitalWrite(LED, LOW);
}

// Public
void EMSChannel::activateChannel(int I, int D) {
    digitalWrite(sw1, HIGH);
    digitalWrite(sw2, HIGH);
    digitalWrite(LED, HIGH);

    intensity = (I/100) * 255;

    pot->write(wiper, intensity);
    delay(D);

    digitalWrite(sw1, LOW);
    digitalWrite(sw2, LOW);
    digitalWrite(LED, LOW);
}