// открытие двери по нажатию на кнопку
void doorOpenInterrupt() 
{
  // если пользователь может зайти
  // подаём напражение на магнит
  digital_write(DOOR_pin, LOW);
  // обновление инфы на сервере
  doorOpen = true;
  // дописать обновление инфы на GyverHUb
}