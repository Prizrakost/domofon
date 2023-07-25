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
#define DOOR_pin 9
#define RED_LED 3
#define GREEN_LED 1

Gwiot7941e rfid;
UnixTime stamp(10);
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
bool doorOpen = false;

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
      Serial.println("Считыватель неисправен");
      //лучше поставить бесконечный цикл, esp можно перезапустить
      break;
      // Ошибку
    }
  }

  // a
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_pin_num)) {
    delay(100);
    Serial.println("initialization failed!");
    //while (1) {delay(10000);}
  }
  root = SD.open("/");
  //printDirectory(root, 0);
  Serial.println("initialization done.");
  if (!(SD.exists("config.txt"))) {
      configure_file();
  }

  // Настройка пинов
  digitalWrite(SOUND_pin_num, HIGH);
  pinMode(SOUND_pin_num, OUTPUT);
  pinMode(BUTTON_pin, INPUT);
  pinMode(DOOR_pin, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  

  read_keys_file();

  read_config_file();
  
  Serial.println("Starting Wi-Fi");
  startWiFi();
  Serial.println("Wi-fFi started");
  configurePortal();
  portal.start();
  
  //если код доходит сюда, то ошибок нет - дверь закрывается
  digitalWrite(DOOR_pin, HIGH);
}

void loop() {
  portal.tick();

  if (rfid.update()) {
    string lastRfidId = String(rfid.getLastTagId());
    if (isCardGranted(lastRfidId)){
      makeLog(lastRfidId + " Access");
      digitalWrite(DOOR_pin, LOW);
      analogWrite(GREEN_LED, 255);
      delay(3000); //думаю, три секунды будет достаточно для открытия двери, она потом примагнитится
      digitalWrite(DOOR_pin, HIGH);
      analoglWrite(GREEN_LED, 0);
      }
    else{
      makeLog(lastRfidId + " Deny");
      analogWrite(RED_LED, 255);
      delay(500);
      analogWrite(RED_LED, 0);
      }
    digitalWrite(SOUND_pin_num, LOW);
    delay(100);
    digitalWrite(SOUND_pin_num, HIGH);
    //printDirectory(root, 0);
  }

}
