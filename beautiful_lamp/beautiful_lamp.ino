// Светильник в москве - d2- лента, d3- реле света, d4 - кнопка

#define NUMBEROBJECTS 8

// ************************** НАСТРОЙКИ ***********************
#define CURRENT_LIMIT 100000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define AUTOPLAY_TIME 60000  // время между сменой режимов в секундах
#define SLEEP_TIME 10800000    // время до выключения по таймеру в мсекундах (3 часа)



#define NUM_LEDS 30         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 1        // количество отрезков ленты (в параллели)
#define LED_PIN 2          // пин ленты
#define BTN_PIN 3          // пин кнопки/сенсора
#define MIN_BRIGHTNESS 70   // минимальная яркость при ручной настройке
#define BRIGHTNESS 120      // начальная яркость


// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************



//-------------------------------------тайменги управления---------------------
#define UDERJAT_DO_VAKLUCHENUA 2000
#define UDERJAT_DO_BELOGO 500
#define ODIN_TAP 500
#define MEJDU_TAPAMI 100


#define MODES_AMOUNT 10

#include "GyverButton.h"
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

#include <FastLED.h>
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

#include "GyverTimer.h"
GTimer_ms effectTimer(60);
GTimer_ms brightTimer(20);

int brightness = BRIGHTNESS;
int tempBrightness;
byte thisMode = 0;
byte saveMode = 0;

//------------------------------------------переменные для управления,для таймеров---------------------
bool lisenButton = true; // переменная для того, чтобы выключать кнопку, когда это требуется, главное не забывать включать ее снова
bool stateButton = false;
bool loging = true; // логирование отладки управления
boolean powerActive = false; // состояние включенно выключенно
boolean whiteMode = false; // режим подсветики белым
bool randomColor = false;
bool timerSleepTF=false; // состояние таймера сна
boolean AutoModeTF = false;
//byte countClicks = 0; // возможно не понадобиться 
long timePress = 0;
long timerSleep = 0;
long timerAutoMode = 0;

bool gReverseDirection = false;

byte hue;
byte brightnesspix;
byte saturation;
byte count;

//------------------------------------------- анимация при переключении -------------------------------
byte action; 
bool flagThenSwitchMode = false;
#define SPEED_ANIMATION 2 // скорость анимации при переключении, клечении, выключении режимов (от 1 до 4)

//-------------------------------------------  обратная связь по включениюЪвыключению таймеров ---------
byte colorFlash = 0; // цвет мигания 
byte countFlash = 0; //сколько вспыхнуть
bool directSpeedFlash = false;
GTimer_ms myTimer(500); // таймер переодичности всыпивания


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

bool oneRun; // НЕ всегда или вообще не работает

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
  touch.setTimeout(ODIN_TAP);
  touch.setStepTimeout(MEJDU_TAPAMI);



  gPal = CRGBPalette16(CRGB::Red, CRGB::Blue, CRGB::Blue,  CRGB::Blue);
  

  switchPower(false); 
}

void loop() {
  if (lisenButton) touch.tick();
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
          if (whiteMode){ // возможно горит берый, который тапом нужно переключить на режим 
            whiteMode = false;
            thisMode = saveMode;
            effectTimer.start();
            if (loging == true) Serial.print("Выключается белый цвет, загарается режим который был до его включения\n"); // вместо этого код для заливки ленты режимом
          }
          else action = 3;
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
          if (whiteMode){ // возможно горит берый, который тапом нужно переключить на режим 
            whiteMode = false;
            thisMode = saveMode;
            effectTimer.start();
            if (loging == true) Serial.print("Выключается белый цвет, загарается режим который был до его включения\n"); // вместо этого код для заливки ленты режимом
          }
          else action = 4;
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
            pullFeedbackTimer(3, 100); // 100 - gren
            if (loging == true) Serial.print("Таймер сна активирован\n");
          }
          else 
          {
            timerSleepTF = false;
            pullFeedbackTimer(1, 1); // 0 - red 1 - почти red но 0 нельзя передавать
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
        if (thisMode != 230 && thisMode != 240){
          if (AutoModeTF == false)
          {
            AutoModeTF = true;
            timerAutoMode = millis();
            pullFeedbackTimer(4, 0); // 100 - green
            whiteMode = false;
            effectTimer.start();
            if (loging == true) Serial.print("Авто смена режимов активирована\n");
          }
          else 
          {
            AutoModeTF = false;
            pullFeedbackTimer(2, 1); // 0 - red
            if (loging == true) Serial.print("Авто смена режимов дизактивирована\n");
          }
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
      stateButton = true;
    }


  if (touch.isRelease()) // разжатие кнопки
  {
    stateButton = false;
    if (millis()- timePress >= UDERJAT_DO_BELOGO && millis()- timePress < UDERJAT_DO_VAKLUCHENUA) 
    {
      if (powerActive)
      {
        if (whiteMode == false)
        {
          whiteMode = true;
          effectTimer.stop();
          saveMode = thisMode;
          thisMode = 230;
          if (loging == true) Serial.print("Включается белый цвет 230й режим.\n"); // вместо этого код для залибки берым цветом ленты
        }
        else 
        {
          if (thisMode == 230){
            whiteMode = true;
            effectTimer.stop(); 
            thisMode = 240;
            if (loging == true) Serial.print("Включается ТУСКЛЫЙ белый цвет 240й режим.\n"); // вместо этого код для залибки берым цветом ленты
          }
          else
          {
            whiteMode = false;
            thisMode = saveMode;
            effectTimer.start();
            if (loging == true) Serial.print("Выключается белый цвет, загарается режим который был до его включения\n"); // вместо этого код для заливки ленты режимом
          }
        }
      }
      else switchPower(true);
        
    }
  }

  if (millis()- timePress >= UDERJAT_DO_VAKLUCHENUA && stateButton)
  {
    if (powerActive) switchPower(false); 
    else 
    {
      switchPower(true);
    }

    stateButton = false;
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
  
 if (loging == true) Serial.print(thisMode);
  if (loging == true) Serial.print("\n");
  if (effectTimer.isReady()) {
    switch (thisMode) {
      case 0: {
        
        colors();
      }
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
      case 9: {
         if (oneRun == true) {
          hue = random(0,255);
          oneRun = false;
         }
        staticColor();//
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
        comets();
      }
        break;
     
      case 14: MoonLight();
        break;
      case 255: feedbackTimer();
        break;
    }
    FastLED.show();
  }

if (thisMode == 230){
  MoonLight();
}
if (thisMode == 240){
  HalfMoonLight();
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
    whiteMode = true;
    effectTimer.stop();
    saveMode = thisMode;
    thisMode = 230;
    if (loging == true) Serial.print("Включается белый цвет.\n"); // вместо этого код для залибки берым цветом ленты
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
        if (brightness - SPEED_ANIMATION > 0) {
          brightness -= SPEED_ANIMATION;
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
        if (brightness + SPEED_ANIMATION < 245){
          brightness += SPEED_ANIMATION;
          FastLED.setBrightness(brightness);
          FastLED.show();
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

void swichMode(bool direct)
{
  if (flagThenSwitchMode == false)
        {
          if (brightness - SPEED_ANIMATION > 0) {
            brightness-=SPEED_ANIMATION;
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
          if (brightness + SPEED_ANIMATION < 249) {
            brightness += SPEED_ANIMATION;
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
        timerSleep = millis();
        if (thisMode >= MODES_AMOUNT) thisMode = 0;
      }
      else 
      {
      
        if (loging == true) Serial.print("Режим <--\n");// thisMode--;
        if (thisMode == 0) thisMode = MODES_AMOUNT - 1;
        else thisMode--;
        timerSleep = millis();
      }

      FastLED.clear();
}



void pullFeedbackTimer(byte cou, byte col){ // сколько раз маргнуть и каким цветом , после того как поморгает вернет текущий режим
  saveMode = thisMode;
  thisMode = 255;
  countFlash = cou;
  colorFlash = col;
  directSpeedFlash = true;
  thisMode = 255;
  brightnesspix = 0;
  if (col == 0){
    randomColor = true;
    colorFlash = random(0,255);
  }
}
