#include <GyverPortal.h>

// Настройки Wi-Fi
bool WiFimode = false; // false = AP, true = STA
String APssid = "domofon";
String APpassword = "domofon123321";
String STAssid;
String STApassword;
GyverPortal portal;

bool doorOpen = false; // Дверь открыта

void build() {
  // конструктор страницы
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  
  GP.SWITCH("Дверь", doorOpen, true);
  
  GP.BUILD_END();
}

void action() {
  // опрос действий
}

void setup() {
  // подключаемся к сети
  Serial.begin(9600);
  startWiFi();
  portal.attachBuild(build);
  portal.attach(action);
  portal.enableAuth("admin", "admin");
  portal.start();
}

void loop() {
  portal.tick();
}
