void configure_file() {
  // Настройка config.txt
  Serial.println("CREATING NEW config.txt");
  config_file = SD.open("config.txt", FILE_WRITE);
  StaticJsonDocument<1024> doc;

  doc["login"] = "admin";
  doc["password"] = "admin";

  doc["WiFimode"] = false;
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
  WiFimode = (bool)doc["WiFimode"];
  APssid = (String)doc["APssid"];
  APpassword = (String)doc["APpassword"];
  STAssid = doc["STAssid"].as<String>();
  STApassword = doc["STApassword"].as<String>();
  config_file.close();
}

void write_config_file() {
  SD.remove("config.txt");
  config_file = SD.open("config.txt", FILE_WRITE);
  StaticJsonDocument<1024> doc;
  doc["login"] = login;
  doc["password"] = password;
  doc["WiFimode"] = WiFimode;
  doc["APssid"] = APssid;
  doc["APpassword"] = APpassword;
  doc["STAssid"] = STAssid;
  doc["STApassword"] = STApassword;
  serializeJson(doc, config_file);
  Serial.print("In vars: ");
  serializeJson(doc, Serial);
  Serial.println();
  Serial.print("In file: ");
  StaticJsonDocument<1024> docq;
  deserializeJson(docq, config_file);
  serializeJson(docq, Serial);
  Serial.println();
  config_file.close();
}

void read_keys_file() {
  keysFile = SD.open("keys.txt", FILE_READ);
  uint8_t key_index = 0;
  while (keysFile.available()) {
    String raw_key = keysFile.readStringUntil('\n');
    cards[key_index].id = getPart(raw_key, ';', 0); //эти массивы потом будут проверяться в соотвтетствии с индексом
    cards[key_index].owner = getPart(raw_key, ';', 1);//то есть если у нас есть совпадение по 4 номеру из cardID
    cards[key_index].permission = ToBool(getPart(raw_key, ';', 2)_;//то надо выводить остальную информацию так же с 4-м номером
    cards[key_index].endDate = ToDate(getPart(raw_key, ';', 4));//например - cardID[4], тогда соответствующие данные - cardOwner[4] ...
    key_index += 1;
  }
  keysFile.close();
}

bool ToBool(data)
{
  if (data == "Yes") return true;
  return false;
}

DateTime ToDate(data)
{
  DateTime res;
  res.getEpochTime.day = getPart(data, '.', 0);
  res.getEpochTime.month = getPart(data, '.', 1);        //я не умею кодить
  res.getEpochTime.year = getPart(data, '.', 2);
  return res;
}




