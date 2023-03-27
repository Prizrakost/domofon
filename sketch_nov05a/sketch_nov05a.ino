#include <Gwiot7941e.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <UnixTime.h>
using namespace std;

#define SD_pin_num 15
#define GWIOT_7941E_RX_PIN 4
#define SOUND_pin_num 2
#define BUTTON_pin 10
Gwiot7941e gwiot7941e;

UnixTime stamp(10);
const char *ssid     = "Pandorum";
const char *password = "TeRRaRiUm2013";

const long utcOffsetInSeconds = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

File root;
File myFile;
File keys;

String cardID[10], cardOwner[10], cardPermission[10], cardDate[10];


void setup() {
  digitalWrite(SOUND_pin_num, HIGH);
  Serial.begin(38400);
  pinMode(SOUND_pin_num, OUTPUT);
  pinMode(BUTTON_pin, INPUT);
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_pin_num)) {
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done.");
  root = SD.open("/");
  printDirectory(root, 0);

  SD.remove("logs/logs.txt");
  
  gwiot7941e.begin(GWIOT_7941E_RX_PIN);

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) 
  {
    delay ( 500 );
    Serial.print ( "." );

    timeClient.begin();
  }
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
  myFile.print(nowTime());
  myFile.print(" ");
  myFile.print(gwiot7941e.getLastTagId(), HEX);
  myFile.print(" ");
  myFile.println(message);
  myFile.close();

  //Вывод сегодняшних логов в serial
  myFile = SD.open(String("logs") + String("/") + _year + String("/") + String(_month) + String("/") + _day + String(".txt"), FILE_READ);
  Serial.println("Reading file...");
  while (myFile.available()) {
  Serial.write(myFile.read());
  }
  myFile.close();
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
