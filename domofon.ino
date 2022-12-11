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
String keys[2][4] = {
  {"1", "2", "1", "4"},
  {"5", "6", "0", "8"}
  };

bool doorOpen = false; // Дверь открыта

void setup() {
  // подключаемся к сети
  Serial.begin(115200);
  startWiFi();
  configurePortal();
  portal.start();
}

void loop() {
  portal.tick();
}
