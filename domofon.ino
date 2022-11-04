#include <Gwiot7941e.h> // RFID-scanner
#include <DS3231.h>     // RTC clock
#include <Wire.h>       // RTC clock
#include <SPI.h>        // SD-card
#include <SD.h>         // SD-card
#include <ESP8266WiFi.h> // Wi-Fi
#include <ESP8266WebServer.h> // Web server

#define GWIOT_7941E_RX_PIN 4 // пин, куда подключён RFID-сканнер
Gwiot7941e gwiot7941e;

char* ssid = "domofon";
char* password = "domofon";
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

DS3231 myRTC;

#define SD_pin_num 15 // пин, куда подключена SD-карта (CS)
File adminPanel;

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
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  // Web server
  server.on("/", handle_OnConnect);
  server.begin();
  Serial.println("HTTP server started");
  adminPanel = SD.open("admin.html");
  // RTC clock setup
  gwiot7941e.begin(GWIOT_7941E_RX_PIN);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gwiot7941e.update()) {
        Serial.println(gwiot7941e.getLastTagId(), HEX);
    }

}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

String SendHTML() {
  String code;
  while (adminPanel.available()) {
      code += adminPanel.read();
    }
  return code;
}
