#define NUMBEROBJECTS 8

// ************************** НАСТРОЙКИ ***********************
#define CURRENT_LIMIT 2000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define AUTOPLAY_TIME 180    // время между сменой режимов в секундах



#define NUM_LEDS 30         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1        // количество отрезков ленты (в параллели)
#define LED_PIN 12           // пин ленты
#define BTN_PIN 3           // пин кнопки/сенсора
#define MIN_BRIGHTNESS 70  // минимальная яркость при ручной настройке
#define BRIGHTNESS 120      // начальная яркость

// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
#define MODES_AMOUNT 15

#include "GyverButton.h"
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN);

#include <FastLED.h>
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

#include "GyverTimer.h"
GTimer_ms effectTimer(60);
GTimer_ms autoplayTimer((long)AUTOPLAY_TIME * 1000);
GTimer_ms brightTimer(20);

int brightness = BRIGHTNESS;
int tempBrightness;
byte thisMode = 11;


bool gReverseDirection = false;
boolean loadingFlag = true;
boolean autoplay = true;
boolean powerDirection = true;
boolean powerActive = false;
boolean powerState = true;
boolean whiteMode = false;
boolean brightDirection = true;
boolean wasStep = false;


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

}

void loop() {
  touch.tick();
  if (touch.hasClicks()) {
    byte clicks = touch.getClicks();
    switch (clicks) {
      case 1:
        powerDirection = !powerDirection;
        powerActive = true;
        tempBrightness = brightness * !powerDirection;
        break;
      case 2: if (!whiteMode && !powerActive) {
          nextMode();
        }
        break;
      case 3: if (!powerActive) {
          whiteMode = !whiteMode;
          if (whiteMode) {
            effectTimer.stop();
            fillAll(CHSV(45, 200, 255),0,NUM_LEDS-1);
            FastLED.show();
          } else {
            effectTimer.start();
          }
        }
        break;
      case 4: if (!whiteMode && !powerActive) autoplay = !autoplay;
        break;
      default:
        break;
    }
  }

  if (touch.isStep()) {
    if (!powerActive) {
      wasStep = true;
      if (brightDirection) {
        brightness += 5;
      } else {
        brightness -= 5;
      }
      brightness = constrain(brightness, MIN_BRIGHTNESS, 255);
      FastLED.setBrightness(brightness);
      FastLED.show();
    }
  }

  if (touch.isRelease()) {
    if (wasStep) {
      wasStep = false;
      brightDirection = !brightDirection;
    }
  }

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
/*
            Serial.println("--------");
            Serial.println(timeToShot[i]);
            Serial.println(directionArry[i]);
            Serial.println(alredyShot[i]);
            Serial.println(colorObject[i]);
            Serial.println(positionArry[i]);*/
            
  
          }
        
          for (int i = NUMBEROBJECTS/2; i < NUMBEROBJECTS; i++) //приспаиваю свойста второй половине объектов
          {
            timeToShot[i] = random(50, 2000); //время до вылета, после обновления таймера, после вылета всех, у каждого объекта оно измениться
            colorObject[i] = 150; //цвет лазерного выстрела присваивается только здесь 150 - наверное синий 
            directionArry[i] = false; //направление выстрела фолс - влево
            alredyShot[i]=false; // возможно это можно будет убрать, потому что при объявлении и так все лож
            positionArry[i] = -1;
          /*
            Serial.println("--------");
            Serial.println(timeToShot[i]);
            Serial.println(directionArry[i]);
            Serial.println(alredyShot[i]);
            Serial.println(colorObject[i]);
            Serial.println(positionArry[i]);*/
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
            if (selectPixArry[i] == false){
              selectPixArry[i] = true;
              colicestvo--;
            }
            if (colicestvo <=0) break;
          }
          fillAll(CHSV(150, 255, 40),0,NUM_LEDS);
          for (int i = 0+NUM_LEDS/10; i <= NUM_LEDS-NUM_LEDS/10; i++){
            if (selectPixArry[i]==true){

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

  if (autoplayTimer.isReady() && autoplay) {    // таймер смены режима
    nextMode();
  }

  brightnessTick();
}

void nextMode() {
  oneRun = true;
  thisMode++;
  if (thisMode >= MODES_AMOUNT) thisMode = 0;
  loadingFlag = true;
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
