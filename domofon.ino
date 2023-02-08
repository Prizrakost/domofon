#include <GyverPortal.h>
#include <ESP8266WiFi.h>

//GyverPortal portal;
GyverPortal portal;
// Брать из файла
config_file = SD.open("config.txt", FILE_WRITE);
void configure_file() {
  // Настройка config.txt
  DynamicJsonDocument doc(1024);

  doc["login"] = "admin";
  doc["password"] = "admin";
  
  doc["APssid"][0] = "domofon";
  doc["APpassword"][1] = "domofon123321";

  doc["STAssid"] = "";
  doc["STApassword"] = "";

  serializeJson(doc, config_file);
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
  // получение config файлов
  if (!SD.exists(config_file)) {
      configure_file();
  }
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  const char* login = doc["login"];
  const char* password = doc["password"];
  String APssid = doc["APssid"];
  String APpassword = doc["APpassword"];
  String STAssid = doc["STAssid"];
  String STApassword = doc["STApassword"];
  
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
