void configure_file() {
  // Настройка config.txt
  config_file = SD.open("config.txt", FILE_WRITE);
  StaticJsonDocument<1024> doc;

  doc["login"] = "admin";
  doc["password"] = "admin";
  
  doc["APssid"] = "domofon";
  doc["APpassword"] = "domofon123321";

  doc["STAssid"] = "";
  doc["STApassword"] = "";

  serializeJson(doc, config_file);
  config_file.close();
}

void read_config_file(){
  config_file = SD.open("config.txt");
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, config_file);
  serializeJson(doc, Serial);
  String loginString = (String)doc["login"];
  loginString.toCharArray(login, 21);
  String passwordString = (String)doc["password"];
  passwordString.toCharArray(password, 21);
  APssid = (String)doc["APssid"];
  APpassword = (String)doc["APpassword"];
  STAssid = doc["STAssid"].as<String>();
  STApassword = doc["STApassword"].as<String>();
  config_file.close();
}

void read_keys_file() {
  keysFile = SD.open("keys.txt", FILE_READ);
  uint8_t key_index = 0;
  while (keysFile.available()) {
    String raw_key = keysFile.readStringUntil('\n');
    cardID[key_index] = (getPart(raw_key, ';', 0)); //эти массивы потом будут проверяться в соотвтетствии с индексом
    cardOwner[key_index] = (getPart(raw_key, ';', 1));//то есть если у нас есть совпадение по 4 номеру из cardID
    cardPermission[key_index] = (getPart(raw_key, ';', 2));//то надо выводить остальную информацию так же с 4-м номером
    cardDate[key_index] = getPart(raw_key, ';', 4);//например - cardID[4], тогда соответствующие данные - cardOwner[4] ...
    key_index += 1;
  }
  keysFile.close();
}