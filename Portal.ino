void build() {
  // конструктор страницы
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.UPDATE("door");

  GP.NAV_TABS("Управление,Настройки Wi-Fi,Логи,Аутентификация");

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Дверь");
  GP.SWITCH("door", doorOpen); //false by default
  GP.NAV_BLOCK_END();

  // Wi-Fi
  GP.NAV_BLOCK_BEGIN();
  GP.FORM_BEGIN("/wifi");    // начать форму, передать имя
  GP.SWITCH("wifimode", WiFimode);
  GP.TEXT("ssid", "SSID");
  GP.PASS("wifipassword", "Password");
  GP.BREAK();                        // перенос строки
  GP.SUBMIT("Подтвердить");         // кнопка Submit
  GP.FORM_END();
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Log");
  GP.EMBED("/log.txt");
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.FORM_BEGIN("/auth");
  GP.TEXT("login", "Логин", login);
  GP.PASS("password", "Пароль", password);
  GP.BREAK();
  GP.SUBMIT("Подтвердить");
  GP.FORM_END();
  GP.NAV_BLOCK_END();

  GP.BUILD_END();
}

void action() {
  // опрос действий
  if (portal.update()) {
    portal.updateBool("door", WiFimode);
  }

  if (portal.click()) {
    // опрос кликов
    Serial.println(portal.clickName());
    if (portal.click("door")) {
      doorOpen = portal.getBool();
      Serial.println(doorOpen);
    }
  }

  if (portal.form()) {
    Serial.print("Submit form: ");

    if (portal.form("/auth")) {
      Serial.println("auth");

      // Записать в файл portal.getString("login")
      //login = portal.getString("login");
      Serial.print("New login: ");
      Serial.println(login);
      // Записать в файл portal.getString("password")
      //password = portal.getString("password");
      Serial.print("New password: ");
      Serial.println(password);
    }

    if (portal.form("/wifi")) {
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

  if (portal.download()) {
    // есть запрос, смотрим адрес (путь к файлу)
    Serial.println(portal.uri());
    portal.sendFile(portal.uri());
  }
}

void configurePortal() {
  portal.attachBuild(build);
  portal.attach(action);
  portal.enableAuth(login, password);
}
