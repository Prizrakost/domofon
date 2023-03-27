#include <GyverPortal.h>
#include <ESP8266WiFi.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

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
  Serial.begin(115200);
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
  
  // подключаемся к сети
  /*
  for (int i = 0; i<sizeof(keys); i++) {
    keys[i][0] = "";
    keys[i][1] = "";
    keys[i][2] = "";
    keys[i][3] = "";
  } */
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
}
