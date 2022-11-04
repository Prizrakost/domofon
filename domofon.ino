#include <Gwiot7941e.h> // RFID-scanner
#include <DS3231.h>     // RTC clock
#include <Wire.h>       // RTC clock
#include <SPI.h>        // SD-card
#include <SD.h>         // SD-card

#define GWIOT_7941E_RX_PIN 4 // пин, куда подключён RFID-сканнер
Gwiot7941e gwiot7941e;

DS3231 myRTC;

#define SD_pin_num 15 // пин, куда подключена SD-карта (CS)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // RTC clock setup
  gwiot7941e.begin(GWIOT_7941E_RX_PIN);
  Wire.begin();

  // SD-card setup
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_pin_num)) {
    Serial.println("initialization failed!");
    // while (1);
  }
  Serial.println("initialization done.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gwiot7941e.update()) {
        Serial.println(gwiot7941e.getLastTagId(), HEX);
    }

}
