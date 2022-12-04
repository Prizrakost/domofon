void build() {
  // конструктор страницы
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.UPDATE("door,log");

  GP.NAV_TABS("Управление,Настройки Wi-Fi,Логи,Ключи,Аутентификация");

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
  GP.AREA("log", 100, logText);
  GP.NAV_BLOCK_END();

  // Ключи
  GP.NAV_BLOCK_BEGIN();
  GP.FORM_BEGIN("/keys");
  GP.TABLE_BEGIN();
  for (int i = 0; i<sizeof(keys); i++) {
    for (int j = 0; i<4; i++) {
      GP.TR();
      GP.TD();
      if (j == 2) {
        GP.CHECK("access#" + String(i), bool(keys[i][2]));
      } else {
        GP.TEXT("", "", keys[i][j]);
      }
    }
  }
  GP.TABLE_END();
  GP.BREAK();
  GP.TEXT("ID");
  GP.TEXT("name");
  GP.TEXT("access");
  GP.TEXT("time");
  GP.BREAK();
  GP.SUBMIT("Добавить");
  GP.FORM_END();
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
    portal.updateBool("door", doorOpen);
    if (portal.update("log")) {
      // Читать из файла
      portal.answer(logText);
    }
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

    if (portal.form("/keys")) {
      Serial.println("keys");

      // Добавить в файл если не пустое
      /*
      portal.getString("ID");
      portal.getString("name");
      portal.getBool("access");
      portal.getString("time");
      */

      // Изменить доступ, где надо
      if (portal.clickSub("access#")) {
        Serial.println(portal.clickName()); // возвращает access#номер ключа
        keys[int(portal.clickName()[7])][2] = char(int(portal.getBool(portal.clickName())));
      }
    }

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
