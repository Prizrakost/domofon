

void setup() {
  

  



  
  Serial.println();
  /*----------------------------тестовый файл keys-------------------------------
  SD.remove("keys.txt");
  keys = SD.open("keys.txt", FILE_WRITE);
  keys.print("15140534;Максим Тескак Марцинкеевич;1");
  keys.close();
  -----------------------------------------------------------------------------*/
  keys = SD.open("keys.txt", FILE_READ);
  uint8_t key_index = 0;
  while (keys.available()) {
    String raw_key = keys.readStringUntil('\n');
    cardID[key_index] = (getPart(raw_key, ';', 0)); //эти массивы потом будут проверяться в соотвтетствии с индексом
    cardOwner[key_index] = (getPart(raw_key, ';', 1));//то есть если у нас есть совпадение по 4 номеру из cardID
    cardPermission[key_index] = (getPart(raw_key, ';', 2));//то надо выводить остальную информацию так же с 4-м номером
    cardDate[key_index] = getPart(raw_key, ';', 4);//например - cardID[4], тогда соответствующие данные - cardOwner[4] ...
    key_index += 1;
  }
  keys.close();
}

void loop() {
  if (gwiot7941e.update()) {
    if (isCardGranted(String(gwiot7941e.getLastTagId()))){
      makeLog("Access");
      }
    else{
      makeLog("Deny");
      }
    digitalWrite(SOUND_pin_num, LOW);
    delay(100);
    digitalWrite(SOUND_pin_num, HIGH);
    printDirectory(root, 0);
   }
}


