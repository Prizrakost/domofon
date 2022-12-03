#include <Gwiot7941e.h> // RFID-scanner
#include <DS3231.h>     // RTC clock
#include <Wire.h>       // RTC clock
#include <SPI.h>        // SD-card
#include <SD.h>         // SD-card
#include <ESP8266WiFi.h> // Wi-Fi
#include <ESP8266WebServer.h> // Web server

class Config {
  public:
    bool WiFimode = false; // false = AP, true = STA
    String APssid = "domofon";
    String APpassword = "domofon123321";
    String STAssid;
    String STApassword;
  private:
};

Config cfg;

#define GWIOT_7941E_RX_PIN 4 // пин, куда подключён RFID-сканнер
Gwiot7941e gwiot7941e;

ESP8266WebServer server(80);

DS3231 myRTC;

#define SD_pin_num 15 // пин, куда подключена SD-карта (CS)
File adminPanelFile;
String adminPanelCode;

bool door = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // SD-card setup
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_pin_num)) {
    Serial.println("initialization failed!");
    // while (1);
  }
  Serial.println("initialization done.");
  // Wi-Fi
  adminPanelFile = SD.open("admin.html"); 
  startWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gwiot7941e.update()) {
        Serial.println(gwiot7941e.getLastTagId(), HEX);
    }

}
