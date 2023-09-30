class KeyCard
{
  public:
    String id;
    String owner;
    bool permission;
    DateTime endDate;

    bool isCardGranted(String readedCardNum)
    {
      for(uint8_t i = 0; i<10; i++){
      if (readedCardNum == id && permission)
        {
          if (ntpCorrect)
          {
            if(endDate.getUnix()/*тут юникс время сравниваем*/ == timeClient.getDateTime(stamp.getUnix())) return true; //прикол тот же, что и с isCardGranted в logging, я не помню как .
            else return false;
          }
          return true;
        }
      }
    return false;
    }

    String StringDate()//метод перевода в формат String, для логов.
    {
      String res;
      res += endDate.getEpochTime().day;
      res += ".";
      res += endDate.getEpochTime().month;
      res += ".";
      res += endDate.getEpochTime().year;
      return res;
    }
}
