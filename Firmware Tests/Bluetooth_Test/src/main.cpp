#include <Arduino.h>
#include <AltSoftSerial.h>

AltSoftSerial rn4020;
int WAKE_HW = 2;
int LED = 13;
String response;
String command;

void reset(int WU) {
  digitalWrite(WU, HIGH);
  delay(200);
  digitalWrite(WU, LOW);
  delay(200);
  digitalWrite(WU, HIGH);
  delay(200);
  digitalWrite(WU, LOW);
  delay(200);
  digitalWrite(WU, HIGH);
  delay(200);
  digitalWrite(WU, LOW);
  delay(200);
  digitalWrite(WU, HIGH);
  delay(200);
  digitalWrite(WU, LOW);
  delay(3000);
  
}

bool printResponse() {
  if (rn4020.available() > 0) {
    response = rn4020.readStringUntil('\n');
    response.trim();
    Serial.println(response);
    response = "";
    return true;
  } else {
    Serial.println("No response. Baudrates probably fucked or something else, fuck knows");
    return false;
  }
}

void resetBaudrate() {
  // Reset baudrate to 2400 
  long int baudrates[8] = {2400, 9600, 19200, 38400, 115200, 230400, 460800, 921600};
  for (int i = 0; i <= 8; i++) {
    Serial.print("Checking baudrate: ");
    Serial.print(baudrates[i]);
    Serial.println("...");

    rn4020.begin(baudrates[i]);
    delay(300);
    rn4020.println(F("V"));
    delay(300);
    Serial.print("\tVersion: ");
    printResponse();
        
    rn4020.println(F("SB,0"));
    delay(300);
    bool checkBaud = printResponse();

    if (checkBaud) {
      Serial.print("Found baudrate! Baudrate was: ");
      Serial.println(baudrates[i]);
      Serial.print("Baudrate reset to: ");
      rn4020.println(F("R,1"));
      delay(5000);
      Serial.print("Rebooting device... ");
      printResponse();

      rn4020.begin(2400);
      delay(300);
      Serial.println("Testing baudrate is now 2400...");
      Serial.print("Get version again... Version: ");
      rn4020.println(F("V"));
      delay(300);
      printResponse();
      return;
    }
  }
  Serial.println("Baudrate not found, something is fucked");
}

void deviceName(String name) {
  rn4020.println("S-," + name);
  delay(300);
  Serial.println("Setting device name too:" + name + "...");
  printResponse();
  rn4020.println("R,1");
  delay(5000);
  Serial.println("Rebooting device...");
  printResponse();
  delay(300);
}

void setup() {
  pinMode(WAKE_HW, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(WAKE_HW, HIGH);

  Serial.begin(115200);
  while(!Serial);
  rn4020.begin(2400);
  //deviceName("openFES_");
  delay(3000);
}

void loop() {
  if (rn4020.available() > 0) {
    response = rn4020.readStringUntil('\n');
    response.trim();
    Serial.println(response);
    response = "";
  }
  delay(100);
  if (Serial.available() > 0) {
    command = Serial.readStringUntil('G');
    command.trim();
    Serial.println("Sending command: " + command + " to bluetooth module...");
    rn4020.println(command);
    delay(300);
    printResponse();   
  }
}