#include <GyverPortal.h>
#include <LittleFS.h>

// Настройки Wi-Fi
bool WiFimode = false; // false = AP, true = STA
String APssid = "domofon";
String APpassword = "domofon123321";
String STAssid;
String STApassword;

//GyverPortal portal;
GyverPortal portal(&LittleFS);

bool doorOpen = false; // Дверь открыта

void setup() {
  // подключаемся к сети
  Serial.begin(9600);
  LittleFS.begin();
  startWiFi();
  configurePortal();
  portal.start();
}

void loop() {
  portal.tick();
}
