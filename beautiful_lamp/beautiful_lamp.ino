/*
  Скетч к проекту "Эффектный светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/gyverlight/
  Исходники на GitHub: https://github.com/AlexGyver/gyverlight/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  https://AlexGyver.ru/
*/
/*
   Управление кнопкой/сенсором
  - Удержание - яркость
  - 1х тап - вкл/выкл
  - 2х тап - переключ режима
  - 3х тап - вкл/выкл белый свет
  - 4х тап - старт/стоп авто смены режимов
*/

/*
   Версия 1.3 - пофикшен баг с fillAll
*/

// ************************** НАСТРОЙКИ ***********************
#define CURRENT_LIMIT 2000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define AUTOPLAY_TIME 60    // время между сменой режимов в секундах
#define time_timer 3600000  // времся с мрмента акимвации таймера выключения
#define NUM_LEDS 30         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 2        // количество отрезков ленты (в параллели)
#define LED_PIN 2           // пин ленты
#define BTN_PIN 4           // пин кнопки/сенсора
#define RELAY_PIN 3          // пин реле основного света
#define MIN_BRIGHTNESS 5  // минимальная яркость при ручной настройке
#define BRIGHTNESS 20      // начальная яркость
#define FIRE_PALETTE 3     
// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
#define MODES_AMOUNT 11

#include "GyverPower.h"

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
byte thisMode = 0;
long timer = 0;
byte saveMode = 0;
byte i = 0;

bool gReverseDirection = false;
boolean loadingFlag = true;
boolean autoplay = false;
boolean timer_on = false;
boolean powerDirection = true;
boolean powerActive = false;
boolean powerState = true;
boolean whiteMode = false;
boolean brightDirection = true;
boolean wasStep = false;


// залить все
void fillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
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
  pinMode(RELAY_PIN, OUTPUT);


  randomSeed(analogRead(0));
  touch.setTimeout(300);
  touch.setStepTimeout(50);


}

void loop() {
  touch.tick();
  if (touch.hasClicks()) {
    byte clicks = touch.getClicks();
    switch (clicks) {
      case 1:
        if (!whiteMode && !powerActive) {
          nextMode();
        }
        break;
      case 2: 
      powerDirection = !powerDirection;
        powerActive = true;
        tempBrightness = brightness * !powerDirection;
        break;
      case 3: if (!powerActive) {
          whiteMode = !whiteMode;
          if (whiteMode) {
            digitalWrite(RELAY_PIN, 1);
          } else {
            digitalWrite(RELAY_PIN, 0);
          }
        }
        break;
      case 4: if (!whiteMode && !powerActive) { 
        timer_on = !timer_on; 
        timer = millis();
        saveMode = thisMode;
        thisMode = 15;
        }
        break;
      default: { 
        saveMode = thisMode;
        break;
      }
        
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
      case 0: fire_4();
        break;
      case 1: fire_3();
        break;
      case 2: fire_1();
        break;
      case 3: fire_2();
        break;
      case 4: rainbow();
        break;
      case 5: sparkles();
        break;
      case 6: colors();
        break;
      case 7: lightBugs();
              break;
      case 8: color_red();
              break;
      case 9: color_aqua();
              break;
      case 10: color_pink();
              break; 
      case 15: tab_for_efect();
              break;
      case 30: the_end();
              break;        
    }
    FastLED.show();
  }

  if (autoplayTimer.isReady() && autoplay) {    // таймер смены режима
  }

  brightnessTick();

  if (timer_on) if (millis()-timer > time_timer){
    thisMode = 30;
} 

  
}
void nextMode() {
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
