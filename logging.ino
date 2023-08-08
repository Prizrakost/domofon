//проверка карты на пропуск
bool isCardGranted(String readedCardNum)
  {
    for(uint8_t i = 0; i<10; i++){
      if (readedCardNum == cardID[i]) return true;
      }
    return false;
  }

//вывод содержимого SD-карты в serial
void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

//получаем часть строки, указывая разделитель
String getPart(String text, char del, int index)
{
  int found = 0;
  int sInd[] = {0, -1};
  int mInd = text.length()-1;
  for(int i=0; i<=mInd && found<=index; i++){
    if(text.charAt(i)==del || i==mInd){
        found++;
        sInd[0] = sInd[1]+1;
        sInd[1] = (i == mInd) ? i+1 : i;
    }
  }
  return found>index ? text.substring(sInd[0], sInd[1]) : "";
}

//создание лога
void makeLog(String message){
  SD.mkdir("logs");

  String _day;
  String _month;
  String _year;

  timeClient.update();
  stamp.getDateTime(timeClient.getEpochTime());
  
  _day = String(stamp.day);
  _month = String(stamp.month);
  _year = String(stamp.year);
  
  Serial.println(_year);

  SD.mkdir(String("logs") + String("/") + _year);
  SD.mkdir(String("logs") + String("/") + _year + String("/") + String(_month));
  
  myFile = SD.open(String("logs") + String("/") + _year + String("/") + String(_month) + String("/") + _day + String(".txt"), FILE_WRITE);
  String nowTime;
  if(ntpCorrect){
    nowTime = nowTime();
  }
  else{
    nowTime = "00:00:00";
  }
  myFile.print(nowTime);
  myFile.print(" ");
  myFile.println(message);
  myFile.close();

  //Вывод сегодняшних логов в serial
  myFile = SD.open(String("logs") + String("/") + _year + String("/") + String(_month) + String("/") + _day + String(".txt"), FILE_READ);
  Serial.println("Reading file...");
  //while (myFile.available()) {
  //Serial.write(myFile.read());
  //}
  String _toWrite = "";
  while (myFile.available()) {
    _toWrite += (char)myFile.read();
  }
  logText = _toWrite;
  myFile.close();
}

// чтение из лога
String readLog()
{
  String _day;
  String _month;
  String _year;


  timeClient.update();
  stamp.getDateTime(timeClient.getEpochTime());

  _day = String(stamp.day);
  _month = String(stamp.month);
  _year = String(stamp.year);

  String q = "";
  char w;
  myFile = SD.open(String("logs") + String("/") + _year + String("/") + String(_month) + String("/") + _day + String(".txt"), FILE_READ);
  Serial.println("Reading file...");
  while (myFile.available()) {
    w = myFile.read();
    if (w != -1)
    {
      q += (char)w;
    }
    else
    {
      break;
    }
  }
  myFile.close();
  return q;
}

//получение даты через ntp сервер
String nowDate()
{
  String res;
  timeClient.update();
  
  stamp.getDateTime(timeClient.getEpochTime());

  res+=stamp.day;
  res+=":";
  res+=stamp.month;
  res+=":";
  res+=stamp.year;
  
  return res;
}

//получение времени через ntp сервер
String nowTime()
{
  String res;
  timeClient.update();
  
  stamp.getDateTime(timeClient.getEpochTime());

  res+=stamp.hour;
  res+=":";
  res+=stamp.minute;
  res+=":";
  res+=stamp.second;
  
  return res;
}
