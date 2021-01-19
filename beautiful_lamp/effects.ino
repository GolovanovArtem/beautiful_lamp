#define TRACK_STEP 50

byte colArry[NUM_LEDS];
byte saturationArry[NUM_LEDS];
byte brightnesspixArry[NUM_LEDS];

// по сути параметры объекта
#define numberObjects 10
byte positionArry[NUM_LEDS];//позиция на ленте от 1 до колличества светодиодов на ленте
bool directionArry[numberObjects];// направление пиксиля (куда бежит)
// еще цвет объекта, но это colArry, его выше объявил 

byte hue;
byte brightnesspix;
byte saturation;
byte count;
// ****************************** ОГОНЁК ******************************
int16_t position;
boolean direction;

void lighter() {
  hue+=1;
  brightnesspix-=6;
  FastLED.clear();
  if (direction) {
    position++;
    if (position > NUM_LEDS - 2) {
      direction = false;
    }
  } else {
    position--;
    if (position < 1) {
      direction = true;
    }
  }
  leds[position] = CHSV(hue, 255, brightnesspix);
}

// ****************************** СВЕТЛЯЧКИ ******************************
#define MAX_SPEED 30
#define BUGS_AMOUNT 4
int16_t speed[BUGS_AMOUNT];
int16_t pos[BUGS_AMOUNT];
CRGB bugColors[BUGS_AMOUNT];

void lightBugs() {
  if (loadingFlag) {
    loadingFlag = false;
    for (int i = 0; i < BUGS_AMOUNT; i++) {
      bugColors[i] = CHSV(random(0, 9) * 28, 255, 255);
      pos[i] = random(0, NUM_LEDS);
      speed[i] += random(-5, 6);
    }
  }
  FastLED.clear();
  for (int i = 0; i < BUGS_AMOUNT; i++) {
    speed[i] += random(-5, 6);
    if (speed[i] == 0) speed[i] += (-5, 6);

    if (abs(speed[i]) > MAX_SPEED) speed[i] = 0;
    pos[i] += speed[i] / 10;
    if (pos[i] < 0) {
      pos[i] = 0;
      speed[i] = -speed[i];
    }
    if (pos[i] > NUM_LEDS - 1) {
      pos[i] = NUM_LEDS - 1;
      speed[i] = -speed[i];
    }
    leds[pos[i]] = bugColors[i];
  }
}

// ****************************** ЦВЕТА ******************************
void colors() {
  hue += 1;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / 255)), 255, 255);
}

// ****************************** РАДУГА ******************************
void rainbow() {
  hue += 4;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / 50)), 255, 255);
}

// ****************************** КОНФЕТТИ ******************************
void sparkles() {
  byte thisNum = random(0, NUM_LEDS);
  if (getPixColor(thisNum) == 0)
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
  fade();
}

// ****************************** cyberpunk ******************************
#define COOLING  55
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void cyberpunk() {
  random16_add_entropy( random());
  cyberpunkWithPalette(); // run simulation frame, using palette colors
}

void cyberpunkWithPalette()
{
  static byte heat[NUM_LEDS];
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[NUM_LEDS - 1 - i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
//brightnesspix

#define MINTIME 3000
#define MAXNTIME 5000

void condominium(){
  hue = 50;
  if (millis()-timermillis1 > chec_time_millis)
  {
      switch (random(0, 8)) 
    {
        case 0: {
          for (int i = 0; i < 5; i++)
             leds[i]= CHSV(hue, 255, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
        case 1: {
          for (int i = 8; i < 13; i++)
             leds[i]= CHSV(hue, 255, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
        case 2: {
          for (int i = 16; i < 21; i++)
             leds[i]= CHSV(hue, 255, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
        case 3: {
          for (int i = 24; i < 29; i++)
             leds[i]= CHSV(hue, 255, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
        case 4: {
          for (int i = 0; i < 5; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
        case 5: {
          for (int i = 8; i < 13; i++)
             leds[i]= CHSV(0, 0, 0);
        }
          break;
        case 6: {
          for (int i = 16; i < 21; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
        case 7: {
          for (int i = 24; i < 29; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
        }
          break;
    }
  }
  

}
void flashescamera() {
  hue += 1;
  chec_time_millis = random(20,400); // частота миганий 
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV((byte)(hue + i * float(255 / 255)), 255, 70);
  }
    if (millis()-timermillis1 > chec_time_millis){
      leds[random(0, NUM_LEDS-1)] = CHSV(hue, 255, 255);
      timermillis1 = millis();
  }
  
}

// *************** ВИНИГРЕТ ***************
void vinigret() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) {
      
    }
  }
}

void laserShot() {
  checkRenderTime = 30;// время отрисовки
  if (millis()-renderTime > checkRenderTime)// ограничение времени отрисовки
    {
      renderTime = millis();
      


     
      







// Записать позиции активных диодов, после чего они будут отрисованы

     // colArry[i] = col;
 // saturationArry[i] = stst;
 // brightnesspixArry[i] = brig;
 //byte positionArry[NUM_LEDS];//позиция на пиксиле от 1 до колличества светодиодов на ленте
//byte directionArry[10];// направление пиксиля (куда бежит)
    }
   for (int i = 0; i < NUM_LEDS; i++)
   {
    leds[i] = CHSV(colArry[i], saturationArry[i], brightnesspixArry[i]);
   }
    
}

void fire () {
  
}
void waterDrop () {
  
}
void comets () {
  
}
void colorCatchUp () {
  if (millis()-timermillis1 > chec_time_millis)// рандомный таймер смены направления режима (при смене направления менять и цвет)
    {
    timermillis1 = millis();
    chec_time_millis = random(3000,1000);//задается время смены направления
    direction = !direction;
    checkRenderTime = random(0,120); // Скорость отображения (скорось бега диодов от 0 до 30 не успевает отрисовывать)
    }
  if (millis()-timerForChangeColor > checkTimeChangeColor)//рандомный таймер смены цета 
  {
    timerForChangeColor = millis();
    checkTimeChangeColor = random(500, 1500);//задается время смены цвета
    hue+=110;//задается цвет
  }
  
  if (direction)
  {
    leds[NUM_LEDS-1] = CHSV(hue, 255, 255);
    synchronizationArrays (NUM_LEDS-1, hue, 255, 255);
    
    if (millis()-renderTime > checkRenderTime)// время отрисовки
    {
      renderTime = millis();
      
      for (int i = 1; i < NUM_LEDS-1; i++)
      {
        leds[i] = CHSV(colArry[i+1], 255, 255); //
        synchronizationArrays (i, colArry[i+1], 255, 255);
      }
      
    }
  }
  else{
    leds[0] = CHSV(hue, 255, 255);
    synchronizationArrays (0, hue, 255, 255);

    if (millis()-renderTime > checkRenderTime)// время отрисовки
    {
      renderTime = millis();
      
      for (int i = NUM_LEDS-1; i > 0; i--)
      {
        leds[i] = CHSV(colArry[i-1], 255, 255); //
        synchronizationArrays (i, colArry[i-1], 255, 255);
      }
      
    }
  }
}
//int16_t position;
//boolean direction;
//byte count;
//byte hue;
//byte brightnesspix;
//byte saturation;

void staticColor () {
  hue = 0;
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(hue, 255, 255);
  }
}
void statikRainbow () {
  hue = 0;
  for (int i = NUM_LEDS; i > 0; i--){
    leds[i] = CHSV(hue, 255, 255);
    hue += 255 / NUM_LEDS;
  }
}

// ****************** СЛУЖЕБНЫЕ ФУНКЦИИ *******************
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);
  }
}

void synchronizationArrays(byte i, byte col, byte stst, byte brig){
  colArry[i] = col;
  saturationArry[i] = stst;
  brightnesspixArry[i] = brig;
}
