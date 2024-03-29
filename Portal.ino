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
  GP.LABEL("Точка доступа");
  GP.SWITCH("wifimode", WiFimode);
  GP.LABEL("Подключение к точке доступа");
  GP.BREAK();
  GP.TEXT("ssid", "SSID");
  GP.PASS("wifipassword", "Password");
  GP.BREAK();                        // перенос строки
  GP.SUBMIT("Подтвердить");         // кнопка Submit
  GP.FORM_END();
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Log");
  GP.AREA("log"), 20, logText, "100%", true;
  GP.NAV_BLOCK_END();

  // Ключи
  GP.NAV_BLOCK_BEGIN();
  GP.FORM_BEGIN("/keys");
  GP.TABLE_BEGIN();
  for (int i = 0; i<sizeof(keys); i++) {
    if (not(keys[i][0] == "")) {
      GP.TR();
    for (int j = 0; j<5; j++) {
      GP.TD();
      switch (j) {
        case 2:
          GP.CHECK("access/" + String(i), bool(keys[i][2]));
          break;
        case 4:
          GP.BUTTON("edit/" + String(i), "Редактировать");
          break;
        default:
          GP.TEXT("", "", keys[i][j]);
          break;
      }
    }
    } else {
      break;
    }
      /*
      if (j == 2) {
        GP.CHECK("access/" + String(i), bool(keys[i][2]));
      } else if (j == 4) {
        GP.BUTTON("edit/" + String(i), "Редактировать");
      } else {
        GP.TEXT("", "", keys[i][j]);
      } */
    }
  GP.TABLE_END();
  GP.BREAK();
  GP.SUBMIT("Записать");
  /*
  GP.BREAK();
  GP.TEXT("ID");
  GP.TEXT("name");
  GP.TEXT("access");
  GP.TEXT("time");
  GP.BREAK();
  GP.SUBMIT("Добавить");
  */
  GP.FORM_END();
  GP.BUTTON("newKey", "Добавить ключ", "", GP_GREEN, "", 0, 1);
  //GP.BUTTON("newKey", "Добавить ключ");
  if (newKeyButton) {
    GP.PROMPT("ID");
    GP.PROMPT("name");
    GP.CONFIRM("access");
    GP.PROMPT("time");
  }
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.FORM_BEGIN("/auth");
  GP.TEXT("login", "Логин", String(login));
  GP.BREAK();
  GP.PASS("password", "Пароль", String(password));
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
      //logText = readLog();
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

    if (portal.click("newKey")) {
      newKeyButton = !newKeyButton;
      Serial.println("newKeyButton: " + String(newKeyButton));
    }

    if (portal.click("ID")) {
      newKey[0] = portal.getString();
      Serial.println("new id: " + newKey[0]);
    }
    if (portal.click("name")) {
      newKey[1] = portal.getString();
      Serial.println("new name: " + newKey[1]);
    }
    if (portal.click("access")) {
      newKey[2] = String(int(portal.getBool()));
      Serial.println("new access: " + newKey[2]);
    }
    if (portal.click("time")) {
      newKey[3] = portal.getString();
      Serial.println("new time: " + newKey[3]);
      for (int i = 0; i<sizeof(keys); i++) {
        if (keys[i][0] == "") {
          keys[i][0] = newKey[0];
          keys[i][1] = newKey[1];
          keys[i][2] = newKey[2];
          keys[i][3] = newKey[3];
          // записать в файл
          break;
        }
      }
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
      // (надеюсь. так сработает)
      // Попользовать “Массив” компонентов? file:///C:/Users/remem/myThings/pandoc-2.19.2/GyverPortal.wiki/%D0%9E%D0%B1%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B0-%D0%BA%D0%BB%D0%B8%D0%BA%D0%BE%D0%B2.html
      // Попользовать кастомную хрень?
      if (portal.clickSub("access")) {
        Serial.println(portal.clickName() + "/" + portal.clickNameSub(1)); // возвращает access/номер ключа
        keys[portal.clickNameSub(1).toInt()][2] = char(int(portal.getBool()));
        
      } else if (portal.clickSub("edit")) {
        Serial.println(portal.clickName() + "/" + portal.clickNameSub(1)); // Придумать сособ редактирования
        /*
         * Строки добавления перенести вверх таблицы
         * Туда кидать значения и редачить
         * Менять действия кнопки Записать? Она ж submit
         * Обновлять текстополя на возможность записывать переменной?
         */
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

      if (!WiFimode) {
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
      write_config_file();
    }
  }
/*
 *if (portal.download()) {
    // есть запрос, смотрим адрес (путь к файлу)
    Serial.println(portal.uri());
    portal.sendFile(portal.uri());
  } 
 */
  
}

void configurePortal() {
  portal.attachBuild(build);
  portal.attach(action);
  portal.enableAuth(login, password);
}
