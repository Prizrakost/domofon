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

Gwiot7941e rfid;
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

String logText = ""; // Лог
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
  rfid.begin(GWIOT_7941E_RX_PIN);
  int i = 0;
  Serial.println("Ждём проверки карты в течении 10 сек.");
  while (!rfid.update())
  {
    i++;
    delay(10);
    // Мигнуть светодиодом
    if (i > 1000)
    {
      Serial.println("RFID сдох");
      break;
      // Ошибку
    }
  }

  // a
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_pin_num)) {
    delay(100);
    Serial.println("initialization failed!");
    while (1) {delay(10000);}
  }
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("initialization done.");
  if (!(SD.exists("config.txt"))) {
      configure_file();
  }

  // Настройка пинов
  digitalWrite(SOUND_pin_num, HIGH);
  pinMode(SOUND_pin_num, OUTPUT);
  pinMode(BUTTON_pin, INPUT);

  read_keys_file();

  read_config_file();
  
  Serial.println("Starting Wi-Fi");
  startWiFi();
  Serial.println("Wi-fFi started");
  configurePortal();
  portal.start();
}

void loop() {
  portal.tick();

  if (rfid.update()) {
    if (isCardGranted(String(rfid.getLastTagId()))){
      makeLog(String(rfid.getLastTagId()) + " Access");
      }
    else{
      makeLog(String(rfid.getLastTagId()) + " Deny");
      }
    digitalWrite(SOUND_pin_num, LOW);
    delay(100);
    digitalWrite(SOUND_pin_num, HIGH);
    //printDirectory(root, 0);
  }

}
