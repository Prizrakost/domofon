#include <GyverPortal.h>
//#include <LittleFS.h>
#include <ESP8266WiFi.h>

// Настройки Wi-Fi
bool WiFimode = false; // false = AP, true = STA
String APssid = "domofon";
String APpassword = "domofon123321";
String STAssid;
String STApassword;

//GyverPortal portal;
GyverPortal portal;
/*
String login = "admin";
String password = "admin";
*/
// Брать из файла
const char* login = "admin";
const char* password = "admin";
String logText = "qwe"; // Лог
String keys[200][4] = {
  {"1", "2", "1", "4"},
  {"5", "6", "0", "8"}
  };
bool newKeyButton = false;
String newKey[4] = {"", "", "", ""};

bool doorOpen = false; // Дверь открыта

void setup() {
  // подключаемся к сети
  Serial.begin(115200);
  Serial.println("Serial began"); /*
  for (int i = 0; i<sizeof(keys); i++) {
    keys[i][0] = "";
    keys[i][1] = "";
    keys[i][2] = "";
    keys[i][3] = "";
  } */
  Serial.println("Starting Wi-Fi");
  startWiFi();
  Serial.println("Wi-fFi started");
  configurePortal();
  portal.start();
}

void loop() {
  portal.tick();
}
