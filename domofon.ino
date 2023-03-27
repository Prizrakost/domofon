#include <GyverPortal.h>
#include <ESP8266WiFi.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <Gwiot7941e.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <UnixTime.h>

#define SD_pin_num 15
#define GWIOT_7941E_RX_PIN 4
#define SOUND_pin_num 2
#define BUTTON_pin 10

Gwiot7941e gwiot7941e;
UnixTime stamp(10);
/*
const char *ssid     = "Pandorum";
const char *password = "TeRRaRiUm2013";
*/
const long utcOffsetInSeconds = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
File root;
File myFile;
File keysFile;
String cardID[10], cardOwner[10], cardPermission[10], cardDate[10];
GyverPortal portal;
File config_file;
bool WiFimode = false; // false = AP, true = STA
String APssid;
String APpassword;
String STAssid;
String STApassword;
char login[21];
char password[21];

String logText = "qwe"; // Лог
String keys[200][4] = {
  {"1", "2", "1", "4"},
  {"5", "6", "0", "8"}
  };
bool newKeyButton = false;
String newKey[4] = {"", "", "", ""};

bool doorOpen = false; // Дверь открыта

void setup() {
  // Инициализация Serial
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial began");

  // работа с rfid
  gwiot7941e.begin(GWIOT_7941E_RX_PIN);

  // Настройка пинов
  digitalWrite(SOUND_pin_num, HIGH);
  pinMode(SOUND_pin_num, OUTPUT);
  pinMode(BUTTON_pin, INPUT);

  // a
  root = SD.open("/");
  printDirectory(root, 0);

  Serial.print("Initializing SD card...");
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  if (!(SD.exists("config.txt"))) {
      configure_file();
  }

  read_keys_file();

  read_config_file();
  
  Serial.println("Starting Wi-Fi");
  startWiFi();
  Serial.println("Wi-fFi started");
  if (!WiFimode) {
    Serial.println("AP ssid: " + APssid);
    }
  configurePortal();
  portal.start();
}

void loop() {
  portal.tick();

  if (gwiot7941e.update()) {
    if (isCardGranted(String(gwiot7941e.getLastTagId()))){
      makeLog("Access");
      }
    else{
      makeLog("Deny");
      }
    digitalWrite(SOUND_pin_num, LOW);
    delay(100);
    digitalWrite(SOUND_pin_num, HIGH);
    printDirectory(root, 0);
  }

}
