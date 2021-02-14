#define NUMBEROBJECTS 8

// ************************** НАСТРОЙКИ ***********************
#define CURRENT_LIMIT 2000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define AUTOPLAY_TIME 2000  // время между сменой режимов в секундах
#define SLEEP_TIME 5000     // время до выключения по таймеру



#define NUM_LEDS 30         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1        // количество отрезков ленты (в параллели)
#define LED_PIN 12          // пин ленты
#define BTN_PIN 3           // пин кнопки/сенсора
#define MIN_BRIGHTNESS 70   // минимальная яркость при ручной настройке
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

//-------------------------------------------переменные анимации при переключении-----------------------
int action;
#define SPED_ANIMATION 2 // скорость анимации при переключении, клечении, выключении режимов (от 1 до 4)

boolean loadingFlag = true;
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
        if (powerActive == false) //если сейчас не горит никакой режим, то включить на текущем thisMode
        {
          switchPower(true);
        }
        else  // если уже включен, то нужно переключить режим --->
        {
          if (whiteMode == false) action = 3;
        }
      }
        break;
      case 2:                // 2 тапа - смена режима <---
      
      {
        if (powerActive == false) //если сейчас не горит никакой режим, то включить на текущем thisMode
        {
          switchPower(true);
        }
        else  // если уже включен, то нужно переключить режим <---
        {
          if (whiteMode == false) action = 4;
        }
      }
        break;
      case 3: 
      {
        if (powerActive == false) //если сейчас не горит никакой режим, то включить на текущем thisMode
        {
          switchPower(true);
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
        if (powerActive == false)
       {
        switchPower(true);
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
      if (millis()- timePress >= 400) 
      {
        if (millis()- timePress >= 1000)
        {
          if (powerActive) switchPower(false); 
          else switchPower(true);
        }
        else
        {
            if (whiteMode == false)
            {
              whiteMode = true;
              effectTimer.stop();
              fillAll(CHSV(45, 180, 255),0,NUM_LEDS);
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
        if (powerActive == false)
         {
          switchPower(true);
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
    switchPower(false);
  }
  if (whiteMode == true) timerAutoMode = millis();
 

  if (effectTimer.isReady()) {
    switch (thisMode) {
      case 0: colors();
        break;
      case 1: rainbow();
        break;
      case 2: cyberpunk();
        break;
      case 3: colorCatchUp();
        break;
      case 4: flashescamera();
        break;
      case 5: lightBugs();
        break;
      case 6: lighter();
        break;
      case 7: sparkles();
        break;
      case 8: statikRainbow();
        break;
      case 9: staticColor();//newrwgim
        break;
      case 10: condominium();
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
        comets();
      }
        break;
      case 13: fire();
        break;
    }
    FastLED.show();

  }


  if (action != 0) animationBrigtness();

}

void  switchPower(bool offOrOn)
{
  if (offOrOn)
  {
    action = 2;
    timerSleepTF = false;
    powerActive = true;
    tempBrightness = brightness * !powerActive;
    if (loging == true) Serial.print("Включить светильник (On). \n");
  }
  else
  {
    timerSleepTF = false;
    action = 1;
    powerActive = false;
    tempBrightness = brightness * !powerActive;
    if (loging == true) Serial.print("Выключить светильник (Off).\n");
  }
}


void animationBrigtness() //action принимает значения 1 - fade / 2 - burn / 3 - rigth / 4 - left  / 0 - nothing
{
  switch (action) 
  {
      case 1:
      {
        if (brightness - SPED_ANIMATION > 0) {
          brightness -= SPED_ANIMATION;
          FastLED.setBrightness(brightness);
          FastLED.show();
        }
        else 
        {
          brightness = 0;
          FastLED.setBrightness(brightness);
          FastLED.show();
          action = 0;
        }
      }
        break;
      case 2: 
      {
        if (brightness + SPED_ANIMATION < 245){
          brightness += SPED_ANIMATION;
          FastLED.setBrightness(brightness);
          FastLED.show();
          Serial.println("Увеличиваю яркость");
        }
        else action = 0;
      }
        break;
      case 3: swichMode(true);
        break;
      case 4: swichMode(false);
        break;
  }
}
bool flagThenSwitchMode = false;

void swichMode(bool direct)
{
  if (flagThenSwitchMode == false)
        {
          if (brightness - SPED_ANIMATION > 0) {
            brightness-=SPED_ANIMATION;
            FastLED.setBrightness(brightness);
            FastLED.show();
          }
          else 
          {
            anotherMode(direct);
            flagThenSwitchMode = true;
          }
        }
        else
        {
          if (brightness + SPED_ANIMATION < 249) {
            brightness += SPED_ANIMATION;
            FastLED.setBrightness(brightness);
            FastLED.show();
          }
          else 
          {
            action = 0;
            flagThenSwitchMode = false;
          }
        }
       
}
void anotherMode(bool direct) {
      loadingFlag = true;
      oneRun = true;
      if (direct)
      {
        if (loging == true) Serial.print("Режим -->\n");// thisMode++;
        thisMode++;
        if (thisMode >= MODES_AMOUNT) thisMode = 0;
      }
      else 
      {
      
        if (loging == true) Serial.print("Режим <--\n");// thisMode--;
        if (thisMode == 0) thisMode = MODES_AMOUNT - 1;
        else thisMode--;
      }

      FastLED.clear();
}
/*
void brightnessTick() {
  if (powerActive) {
    if (brightTimer.isReady()) {
      if (powerActive) {
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
*/
