
/*
  LsCompanion Firmware
  The MIT License (MIT)
  Copyright (c) 2020 Simon Bogutzky
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
  v1.0.0
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NewPing.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

const int displayResetPin = 18;
const int displayWidth = 128;
const int displayHeight = 64;

Adafruit_SSD1306 display(displayWidth, displayHeight, &Wire, displayResetPin);

const int ultrasonicSensorTriggerPin = 32;
const int ultrasonicSensorEchoPin = 33;
const int ultrasonicSensorMaxDistance = 150; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
const int ultrasonicSensorMedianIterations = 10;

NewPing sonar(ultrasonicSensorTriggerPin, ultrasonicSensorEchoPin, ultrasonicSensorMaxDistance); 

int tableOffsetInCentimeter = 4;
int tableHeightInCentimeter;

const char* bleTableServiceUuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* bleTableHeightCharacteristicUuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
BLECharacteristic *bleTableHeightCharacteristic;
bool bleClientConnected = false;

class BleTableServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* server) {
      bleClientConnected = true;
      Serial.print("Client connected");
    };
 
    void onDisconnect(BLEServer* server) {
      bleClientConnected = false;
      Serial.print("Client disconnected");
    }
};

void setup() {
  delay(1000);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay(); 
  
  pinMode(ultrasonicSensorTriggerPin, OUTPUT);
  pinMode(ultrasonicSensorEchoPin, INPUT);

  BLEDevice::init("LS Companion");
  BLEServer *bleTableServer = BLEDevice::createServer();
  bleTableServer->setCallbacks(new BleTableServerCallbacks());
  BLEService *bleTableService = bleTableServer->createService(bleTableServiceUuid);
  bleTableHeightCharacteristic = bleTableService->createCharacteristic(
                                         bleTableHeightCharacteristicUuid,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  bleTableHeightCharacteristic->setValue("0");
  bleTableService->start();
  BLEAdvertising *bleTableServerAdvertising = bleTableServer->getAdvertising();
  bleTableServerAdvertising->start();

  Serial.begin(9600);
}

void loop() {
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  float duration = sonar.ping_median(ultrasonicSensorMedianIterations);
  int newTableHeightInCentimeter = (int)((duration / 2) * 0.0343) + tableOffsetInCentimeter;

  if (tableHeightInCentimeter != newTableHeightInCentimeter) {
    tableHeightInCentimeter = newTableHeightInCentimeter;

    Serial.print("Table height = ");
    Serial.print(tableHeightInCentimeter);
    Serial.println(" cm");
    
    if (bleClientConnected) { 
      bleTableHeightCharacteristic->setValue(tableHeightInCentimeter); 
      bleTableHeightCharacteristic->notify(); 
    }
  }

  display.clearDisplay();
  String tableHeightString = "";
  tableHeightString.concat(tableHeightInCentimeter);
  tableHeightString.concat(" cm");
  drawText(0, 0, stringToChar(tableHeightString), 2);

  if(bleClientConnected) {
    String bleClientConnectedString = "Client connected";
    drawText(0, 32, stringToChar(bleClientConnectedString), 1);
  }
  
  display.display();
}

char* stringToChar(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void drawText(byte xPos, byte yPos, char *text, byte textSize)
{
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(xPos, yPos);
  display.setTextSize(textSize);
  display.print(text);
}
