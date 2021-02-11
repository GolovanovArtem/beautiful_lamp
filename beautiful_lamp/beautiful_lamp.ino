#define NUMBEROBJECTS 8

// ************************** НАСТРОЙКИ ***********************
#define CURRENT_LIMIT 2000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define AUTOPLAY_TIME 2000    // время между сменой режимов в секундах
#define SLEEP_TIME 5000    // время до выключения по таймеру



#define NUM_LEDS 30         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1        // количество отрезков ленты (в параллели)
#define LED_PIN 12           // пин ленты
#define BTN_PIN 3           // пин кнопки/сенсора
#define MIN_BRIGHTNESS 70  // минимальная яркость при ручной настройке
#define BRIGHTNESS 120      // начальная яркость

// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
#define MODES_AMOUNT 10

#include "GyverButton.h"
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN);

#include <FastLED.h>
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

#include "GyverTimer.h"
GTimer_ms effectTimer(60);
GTimer_ms brightTimer(20);

int brightness = BRIGHTNESS;
int tempBrightness;
byte thisMode = 0;

//------------------------------------------переменные для управления,для таймеров---------------------

bool loging = true; // логирование отладки управления
boolean powerActive = true; // состояние включенно выключенно
boolean whiteMode = false; // режим подсветики белым
bool timerSleepTF=false; // состояние таймера сна
boolean AutoModeTF = false;
//byte countClicks = 0; // возможно не понадобиться 
long timePress = 0;
long timerSleep = 0;
long timerAutoMode = 0;

bool gReverseDirection = false;


boolean loadingFlag = true;
boolean powerDirection = true;
boolean powerState = true;
//boolean brightDirection = true;
//boolean wasStep = false;


int timeToShot[NUMBEROBJECTS];
byte colorObject[NUMBEROBJECTS]; //цвета оъектов
bool directionArry[NUMBEROBJECTS];// направление пиксиля (куда бежит)
bool alredyShot[NUMBEROBJECTS]; // "уже вылетел", на кждый объект, чтобы не вылетали повторно.
int positionArry[NUMBEROBJECTS];//позиция на ленте от 1 до колличества светодиодов на ленте
bool selectPixArry[NUMBEROBJECTS];

byte colicestvo;

bool oneRun;

long timermillis1 = 0;
int chec_time_millis= 0;

long timerForChangeColor = 0;//
int checkTimeChangeColor = 0;//

long renderTime = 0;    //таймер отрисовки
int checkRenderTime = 0;//проверка нужно лти отрисовать

// переменные засекания времени.




// залить все
void fillAll(CHSV newcolor,int i, int l) {
  for (i = 0; i < l; i++) {
    leds[i] = newcolor;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisPixel) {
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT / NUM_STRIPS);
  FastLED.setBrightness(brightness);
  FastLED.show();

  randomSeed(analogRead(0));
  touch.setTimeout(300);
  touch.setStepTimeout(50);



  gPal = CRGBPalette16(CRGB::Red, CRGB::Blue, CRGB::Blue,  CRGB::Blue);
  


  if (loging == true) Serial.print("Светильник включен\n");


}

void loop() {
  touch.tick();
  if (touch.hasClicks()) { // проверка на наличие действий с кнопкой
    byte clicks = touch.getClicks();// считаем колличество тапов по кнопке
    switch (clicks) {                               // один тап - смена режима --->
      case 1:
      {
        if (powerDirection == false) //если сейчас не горит никакой режим, то включить на текущем thisMode
        {
           timerSleepTF = false;
           powerActive = true;
           powerDirection = true;
           tempBrightness = brightness * !powerDirection;
          if (loging == true) Serial.print("Включить светильник\n");
        }
        else  // если уже включен, то нужно переключить режим --->
        {
          anotherMode(true);
          if (loging == true) Serial.print("Режим -->\n");// thisMode++;
        }
      }
        break;
      case 2:                // 2 тапа - смена режима <---
      
      {
        if (powerDirection == false) //если сейчас не горит никакой режим, то включить на текущем thisMode
        {
          timerSleepTF = false;
          powerActive = true;
          powerDirection = true;
          tempBrightness = brightness * !powerDirection;
          if (loging == true) Serial.print("Включить светильник\n");
        }
        else  // если уже включен, то нужно переключить режим <---
        {
          anotherMode(false);
          if (loging == true) Serial.print("Режим <--\n");// thisMode--;
        }
      }
        break;
      case 3: 
      {
        if (powerDirection == false) //если сейчас не горит никакой режим, то включить на текущем thisMode
        {
          timerSleepTF = false;
          powerActive = true;
          powerDirection = true;
          tempBrightness = brightness * !powerDirection;
          if (loging == true) Serial.print("Включить светильник\n");
        }
        else  // если уже включен, то...
        {
          if (timerSleepTF == false)
          {
            timerSleepTF = true;
            timerSleep = millis();
                                                                            // помигать что таймер включен
            if (loging == true) Serial.print("Таймер сна активирован\n");
          }
          else 
          {
            timerSleepTF = false;
                                                                            // помигать что таймер сна отключен
            if (loging == true) Serial.print("Таймер сна дизактивирован\n");
          }
        }
      }
        break;
      case 4: 
      {
        if (powerDirection == false)
       {
        timerSleepTF = false;
        powerActive = true;
        powerDirection = true;
        tempBrightness = brightness * !powerDirection;
        Serial.print("Включить светильник\n");
       }
       else 
       {
        if (AutoModeTF == false)
        {
          AutoModeTF = true;
          timerAutoMode = millis();
                                                                   // помигать что таймер автосмены включен
          if (loging == true) Serial.print("Авто смена режимов активирована\n");
        }
        else 
        {
          AutoModeTF = false;
                                                                  // помигать что таймер автосмены выключен
          if (loging == true) Serial.print("Авто смена режимов дизактивирована\n");
        }
       }
      }
        break;
      default:
        break;
    }
  }

  if (touch.isPress()) // нажатие кнопки
    {
      timePress = millis(); // засекаю время нажатия 
    }


      if (touch.isRelease()) // разжатие кнопки
    {
      if (millis()- timePress >= 700) 
      {
        if (millis()- timePress >= 1700)
        {
          powerDirection = false;
          powerActive = true;
          tempBrightness = brightness * !powerDirection;
          if (loging == true) Serial.print("Выключить светильник.\n");
        }
        else
        {
            if (whiteMode == false)
            {
              whiteMode = true;
              effectTimer.stop();
              fillAll(CHSV(45, 200, 255),0,NUM_LEDS);
              FastLED.show();
              if (loging == true) Serial.print("Включается белый цвет.\n"); // вместо этого код для залибки берым цветом ленты
            }
            else 
            {
              whiteMode = false;
              effectTimer.start();
              if (loging == true) Serial.print("Выключается белый цвет, загарается режим который был до его включения\n"); // вместо этого код для заливки ленты режимом
            }
        }
      }
      else 
      {
        if (powerDirection == false)
         {
          timerSleepTF = false;
          powerActive = true;
          powerDirection = true;
          tempBrightness = brightness * !powerDirection;
          Serial.print("Включить светильник\n");
         }
      }
      
    }

  if (millis() - timerAutoMode > AUTOPLAY_TIME && AutoModeTF == true) 
  {
    timerAutoMode = millis();
    anotherMode(true);
    if (loging == true) Serial.print("Режим --> по таймеру.\n");
  }
  if (millis() - timerSleep > SLEEP_TIME && timerSleepTF == true) 
  { 
    timerSleepTF = false;
    powerDirection = false;
    powerActive = true;
    tempBrightness = brightness * !powerDirection;
    if (loging == true) Serial.print("Выключить светильник по таймеру.\n");
  }
  if (whiteMode == true) timerAutoMode = millis();
 

  if (effectTimer.isReady() && powerState) {
    switch (thisMode) {
      case 0: lighter();
        break;
      case 1: lightBugs();
        break;
      case 2: colors();
        break;
      case 3: rainbow();
        break;
      case 4: sparkles();
        break;
      case 5: cyberpunk();
        break;
      case 6: fire();
        break;
      case 7: condominium();
        break;
      case 8: vinigret();
        break;
      case 9: flashescamera();//newrwgim
        break;
      case 10:
      {
        if (oneRun == true){
          oneRun = false;
          
          for (int i = 0; i < NUMBEROBJECTS/2; i++) //приспаиваю свойста первой половине объектов
          {
            timeToShot[i] = random(50, 2000); //время до вылета, после обновления таймера, после вылета всех, у каждого объекта оно измениться
            colorObject[i] = 0; //цвет лазерного выстрела присваивается только здесь 0 - красный 
            directionArry[i] = true; //направление выстрела тру - вправо
            alredyShot[i]=false; // возможно это можно будет убрать, потому что при объявлении и так все лож
            positionArry[i] = -1;
          }
        
          for (int i = NUMBEROBJECTS/2; i < NUMBEROBJECTS; i++) //приспаиваю свойста второй половине объектов
          {
            timeToShot[i] = random(50, 2000); //время до вылета, после обновления таймера, после вылета всех, у каждого объекта оно измениться
            colorObject[i] = 150; //цвет лазерного выстрела присваивается только здесь 150 - наверное синий 
            directionArry[i] = false; //направление выстрела фолс - влево
            alredyShot[i]=false; // возможно это можно будет убрать, потому что при объявлении и так все лож
            positionArry[i] = -1;
      
          }
          chec_time_millis = millis();
          FastLED.clear();
        }
        laserShot();
      }
        break;
      case 11: waterDrop();//newrwgim
        break;
      case 12: {
        if (oneRun == true){
          oneRun = false;
          colicestvo = 5;
          while(true){
            int i = random(0+NUM_LEDS/10,NUM_LEDS-NUM_LEDS/10);
            if (selectPixArry[i] == false and selectPixArry[i-1] == false and selectPixArry[i+1] == false){
              selectPixArry[i] = true;
              colicestvo--;
            }
            if (colicestvo <= 0) break;
          }
          fillAll(CHSV(150, 255, 40),0,NUM_LEDS);
          for (int i = 0+NUM_LEDS/10; i <= NUM_LEDS-NUM_LEDS/10; i++){
            if (selectPixArry[i] == true){

            }
          }
        }
        comets();//newrwgim
      }
        break;
      case 13: colorCatchUp();
        break;
      case 14: staticColor();
        break;
      case 15: statikRainbow();
        break;
    }
    FastLED.show();
  }

  brightnessTick();
}

void anotherMode(bool direct) {
    loadingFlag = true;
    oneRun = true;
    if (direct){
      thisMode++;
      if (thisMode >= MODES_AMOUNT) thisMode = 0;
    }
    else {
      
      if (thisMode == 0) thisMode = MODES_AMOUNT - 1;
      else thisMode--;
    }
  

  FastLED.clear();
}

void brightnessTick() {
  if (powerActive) {
    if (brightTimer.isReady()) {
      if (powerDirection) {
        powerState = true;
        tempBrightness += 10;
        if (tempBrightness > brightness) {
          tempBrightness = brightness;
          powerActive = false;
        }
        FastLED.setBrightness(tempBrightness);
        FastLED.show();
      } else {
        tempBrightness -= 10;
        if (tempBrightness < 0) {
          tempBrightness = 0;
          powerActive = false;
          powerState = false;
        }
        FastLED.setBrightness(tempBrightness);
        FastLED.show();
      }
    }
  }
}
