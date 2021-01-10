#include "GyverButton.h"
GButton button(2, LOW_PULL, NORM_OPEN);
bool loging = true; // логирование
bool OffOrOn = false; // состояние включенно выключенно
bool whiteLight = false; // вкыл/вкл белый свет (состояние)
bool timerSleepTF=false; // состояние таймера сна
bool AutoModTF = false; // Таймер автосмены режимов
byte countClicks = 0;
long timePress = 0;
void setup() {
  

  Serial.begin(9600);
  OffOrOn = true; 
  if (loging == true) Serial.print("Светильник включен\n");

}

void loop() {
  button.tick(); // опрос состояния кнопки

  if (button.hasClicks())                  // проверка на наличие нажатий
    countClicks = button.getClicks();    // получить и запомнить в  число нажатий
    
  switch(countClicks)
  {
    case 1:
      if (OffOrOn == false) 
      {
        OffOrOn = true;
        if (loging == true) Serial.print("Включить светильник\n");
      }
      else  
      {
        if (loging == true) Serial.print("Режим -->\n");// thisMode++;
        
      }
      countClicks = 0;
      break;
    case 2:
       if (OffOrOn == false) 
       {
        OffOrOn = true;
        if (loging == true) Serial.print("Включить светильник\n");
       }
       else if (loging == true) Serial.print("Режим <--\n");// thisMode--;
      countClicks = 0;
      break;
    case 3:
       if (OffOrOn == false)
       {
        OffOrOn = true;
        Serial.print("Включить светильник\n");
       }
       else 
       {
        if (timerSleepTF == false)
        {
          timerSleepTF = true;
          // помигать что таймер включен
          if (loging == true) Serial.print("Таймер сна активирован\n");
        }
        else 
        {
          timerSleepTF = false;
          if (loging == true) Serial.print("Таймер сна дизактивирован\n");
        }
       }
      countClicks = 0;
      break;
        case 4:
       if (OffOrOn == false)
       {
        OffOrOn = true;
        Serial.print("Включить светильник\n");
       }
       else 
       {
        if (AutoModTF == false)
        {
          AutoModTF = true;
          // помигать что таймер включен
          if (loging == true) Serial.print("Авто смена режимов активирована\n");
        }
        else 
        {
          AutoModTF = false;
          if (loging == true) Serial.print("Авто смена режимов дизактивирована\n");
        }
       }
      countClicks = 0;
      break;
  }

  
  if (button.isPress()) // нажатие кнопки
  {
    timePress = millis(); // засекаю время нажатия 
  }
  if (button.isRelease()) // разжатие кнопки
  {
    if (OffOrOn = true) // если включен и удержали
    {
      if (millis()- timePress >= 700) 
      {
        if (millis()- timePress >= 1700)
        {
          OffOrOn = false;
          if (loging == true) Serial.print("Выключить светильник.\n");
        }
        else
        {
            if (whiteLight == false)
            {
              whiteLight = true;
              if (loging == true) Serial.print("Включается белый цвет.\n"); // вместо этого код для залибки берым цветом ленты
            }
            else 
            {
              whiteLight = false;
              if (loging == true) Serial.print("Выключается белый цвет, загарается режим который был до его включения\n"); // вместо этого код для заливки ленты режимом
            }
        }
      } 
    }
    else // если включен и удержали
    {
      if (millis()- timePress >= 700) 
      {
        whiteLight = true;
        if (loging == true) Serial.print("Выключается белый цвет, разбудиди удержанием \n"); // вместо этого код для заливки ленты режимом
      }
    }
  
    
  }
  
}
