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

//GyverPortal portal;
GyverPortal portal;
// Брать из файла
File config_file;
bool WiFimode = false; // false = AP, true = STA
String APssid;
String APpassword;
String STAssid;
String STApassword;
//String login;
//String password;
char login[21];
char password[21];
void configure_file() {
  // Настройка config.txt
  config_file = SD.open("config.txt", FILE_WRITE);
  StaticJsonDocument<1024> doc;

  doc["login"] = "admin";
  doc["password"] = "admin";
  
  doc["APssid"] = "domofon";
  doc["APpassword"] = "domofon123321";

  doc["STAssid"] = "";
  doc["STApassword"] = "";

  serializeJson(doc, config_file);
  config_file.close();
}

String logText = "qwe"; // Лог
String keys[200][4] = {
  {"1", "2", "1", "4"},
  {"5", "6", "0", "8"}
  };
bool newKeyButton = false;
String newKey[4] = {"", "", "", ""};

bool doorOpen = false; // Дверь открыта

void setup() {
  digitalWrite(SOUND_pin_num, HIGH);
  pinMode(SOUND_pin_num, OUTPUT);
  pinMode(BUTTON_pin, INPUT);
  root = SD.open("/");
  printDirectory(root, 0);

  SD.remove("logs/logs.txt");
  
  gwiot7941e.begin(GWIOT_7941E_RX_PIN);

  Serial.begin(9600);
  Serial.println("Serial began");
  // получение config файлов
  Serial.print("Initializing SD card...");
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  if (!(SD.exists("config.txt"))) {
      configure_file();
  }

  keysFile = SD.open("keys.txt", FILE_READ);
  uint8_t key_index = 0;
  while (keysFile.available()) {
    String raw_key = keysFile.readStringUntil('\n');
    cardID[key_index] = (getPart(raw_key, ';', 0)); //эти массивы потом будут проверяться в соотвтетствии с индексом
    cardOwner[key_index] = (getPart(raw_key, ';', 1));//то есть если у нас есть совпадение по 4 номеру из cardID
    cardPermission[key_index] = (getPart(raw_key, ';', 2));//то надо выводить остальную информацию так же с 4-м номером
    cardDate[key_index] = getPart(raw_key, ';', 4);//например - cardID[4], тогда соответствующие данные - cardOwner[4] ...
    key_index += 1;
  }
  keysFile.close();

  config_file = SD.open("config.txt");
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, config_file);
  serializeJson(doc, Serial);
  String loginString = (String)doc["login"];
  loginString.toCharArray(login, 21);
  String passwordString = (String)doc["password"];
  passwordString.toCharArray(password, 21);
  APssid = (String)doc["APssid"];
  APpassword = (String)doc["APpassword"];
  STAssid = doc["STAssid"].as<String>();
  STApassword = doc["STApassword"].as<String>();
  config_file.close();
  
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
