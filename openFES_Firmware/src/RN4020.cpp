/*
 *  R4020.cpp
 * 
 *  Created on: 20/02/2025
 *  Author: 	Ross Underhill
 * 
 *  Description: Class controlling communication with RN4020
 *               Bluetooth module.
 */

 #include "RN4020.h"

RN4020::RN4020(AltSoftSerial *bt, int wake) {
    this->bluetooth = bt;
    this->wakeHW = wake;
    this->response = "";
    this->baudrates[0] = 2400;
    this->baudrates[1] = 9600;
    this->baudrates[2] = 19200;
    this->baudrates[3] = 38400;
    this->baudrates[4] = 115200;
    this->baudrates[5] = 230400;
    this->baudrates[6] = 460800;
    this->baudrates[7] = 921600;
}

RN4020::~RN4020() {
}

 // Public

void RN4020::wake() {
    digitalWrite(this->wakeHW, HIGH);
}

void RN4020::sleep() {
    digitalWrite(this->wakeHW, LOW);
}

void RN4020::init() {
    this->bluetooth->begin(2400);
    delay(1000);
    this->bluetooth->println("V");
    delay(300);
    Serial.println("RN4020 has been initialised\nVersion: " + getResponse());
}

String RN4020::getResponse() {
    this->response = "";
    if (this->bluetooth->available() > 0) {
        this->response = this->bluetooth->readStringUntil('\n');
        this->response.trim();
    } else {
        this->response = "None";
    }
    return response;
}

bool RN4020::printResponse() {
    this->response = "";
    if (this->bluetooth->available() > 0) {
        this->response = this->bluetooth->readStringUntil('\n');
        this->response.trim();
        Serial.println(this->response);
        return true;
    } else {
        this->bluetooth->println("No response avaialble");
        return false;
    }
}

void RN4020::baudrateReset() {
    Serial.println("Resetting baudrate...");
    for (int i = 0; i < 8; i++) {
        Serial.println("Checking baudrate:" + String(this->baudrates[i]) + "...");
        this->bluetooth->begin(this->baudrates[i]);
        delay(300);
        this->bluetooth->println("V");
        delay(300);
        Serial.println("Version: " + getResponse());
        
        this->bluetooth->println("SB,0");
        delay(300);
        bool checkBaud = printResponse();
        if (checkBaud) {
            Serial.println("Found baudrate! Baudrate reset to 2400");
            this->bluetooth->println("R,1");
            delay(5000);
            Serial.print("Rebooting device...");
            printResponse();

            this->bluetooth->begin(2400);
            delay(300);
            Serial.println("Testing baudrate is now 2400...");
            Serial.print("Get version again... Version: ");
            this->bluetooth->println("V");
            delay(300);
            printResponse();
            return;
        }
    }
    Serial.println("Baudrate reset failed");
}

void RN4020::factoryReset(int resetType) {
    // Reset type 1 = factory reset
    // Reset type 2 = factory reset and clear all settings
    this->bluetooth->println("SF," + resetType);
    delay(200);
    printResponse();
    this->bluetooth->println("R,1");
    delay(5000);
    Serial.print("Rebooting device...");
    printResponse();
    baudrateReset();
}

void RN4020::setName(String name) {
    Serial.println("Setting device name to: " + name + "...");
    this->bluetooth->println("SN," + name);
    delay(200);
    printResponse();
    this->bluetooth->println("R,1");
    delay(5000);
    Serial.print("Rebooting device...");
    printResponse();
    this->bluetooth->begin(2400);
    delay(3000);
}

void RN4020::pair() {
    Serial.println("Pairing device...");
    while (true) {
        if (this->bluetooth->available() > 0) {
            this->response = this->bluetooth->readStringUntil('\n');
            this->response.trim();
            Serial.println(this->response);
            if (this->response == "Bonded") {
                Serial.println("Device connected");
                delay(5000);
                if (this->bluetooth->available() > 0) {
                    this->response = this->bluetooth->readStringUntil('\n');
                    this->response.trim();
                    if (this->response == "Connection End") {
                        Serial.println("Device paired but disconnected immedietely");
                        break;
                    }
                }
            } else if (this->response == "Connection End") {
                Serial.println("Error in pairing device");
                break;
            }
        }
    }
}

void RN4020::unpair() {
    Serial.println("Unpairing device...");
    this->bluetooth->println("U");
    delay(500);
    if (this->bluetooth->available() > 0) {
        this->response = this->bluetooth->readStringUntil('\n');
        this->response.trim();
        if (this->response == "AOK") {
            Serial.println("Device unpaired");
        } else {
            Serial.println("Error in unpairing device");
        }
    }
}

void RN4020::talk() {
    Serial.println("Entering debug mode for RN4020...\nType exit to leave debug mode");
    while (true) {
        String txt = "";
        if (this->bluetooth->available() > 0) {
            txt = this->bluetooth->readStringUntil('\n');
            txt.trim();
            Serial.println(txt);
        }
        if (Serial.available() > 0) {
            txt = Serial.readStringUntil('\n');
            txt.trim();
            if (txt == "exit") {
                Serial.println("Exiting debug mode...");
                break;
            }
            Serial.println("Sending command: " + txt + " to bluetooth module...");
            this->bluetooth->println(txt);
            delay(300);
        }
        delay(1);
    }
}