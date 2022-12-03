#include <GyverPortal.h>

// Настройки Wi-Fi
bool WiFimode = false; // false = AP, true = STA
String APssid = "domofon";
String APpassword = "domofon123321";
String STAssid;
String STApassword;

void setup() {
  Serial.begin(9600);
  startWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
