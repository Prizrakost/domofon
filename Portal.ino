void build() {
  // конструктор страницы
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.UPDATE("door");

  // Wi-Fi
  GP.FORM_BEGIN("/wifi");    // начать форму, передать имя
  GP.SWITCH("wifimode", WiFimode);
  GP.TEXT("ssid", "SSID");
  GP.PASS("wifipassword", "Password");
  GP.BREAK();                        // перенос строки
  GP.SUBMIT("Подтвердить");         // кнопка Submit
  GP.FORM_END(); 

  GP.HR();

  GP.LABEL("Дверь");
  GP.SWITCH("door", doorOpen); //false by default
  GP.BREAK();
  
  GP.BUILD_END();
}

void action() {
  // опрос действий
  if (portal.click()) {
    // опрос кликов
    Serial.println(portal.clickName());
    if (portal.click("door")) {
      doorOpen = portal.getBool();
      Serial.println(doorOpen);
    }
  }

  if (portal.form()){
    Serial.print("Submit form: ");
    if (portal.form("/wifi")){
      Serial.println("wifi");
      
      WiFimode = portal.getBool("wifimode");
      Serial.print("WiFi mode: ");

      if (WiFimode) {
        Serial.println("AP");
        APssid = portal.getString("ssid");
        Serial.print("AP SSID: ");
        Serial.println(APssid);
        APpassword = portal.getString("wifipassword");
        Serial.print("AP Password: ");
        Serial.println(APpassword);
      } else {
        Serial.println("STA");
        STAssid = portal.getString("ssid");
        Serial.print("STA SSID: ");
        Serial.println(STAssid);
        STApassword = portal.getString("wifipassword");
        Serial.print("STA Password: ");
        Serial.println(STApassword);
      }
    }
  }
}

void configurePortal() {
  portal.attachBuild(build);
  portal.attach(action);
  portal.enableAuth("admin", "admin");
}
