#define TRACK_STEP 50

byte colArry[NUM_LEDS];
byte saturationArry[NUM_LEDS];
byte brightnesspixArry[NUM_LEDS];

// по сути параметры объекта




// еще цвет объекта, но это colArry, его выше объявил 
bool shotAll; // флаг что вылетели все, можно обновлять таймер


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
  Serial.println(hue);
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
#define MAXNTIME 7000
// ****************************** Моногоэтажка ******************************
void condominium(){
  hue = 50;
  if (millis()-timermillis1 > chec_time_millis)
  {
      switch (random(0, 11)) 
    {
        case 0: {
          for (int i = 0; i < 5; i++)
             leds[i]= CHSV(45, 200, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
        case 1: {
          if (random(1,10)>6){
            hue = 200;
          }
          else hue = 45;
          for (int i = 6; i < 10; i++)
             leds[i]= CHSV(hue, 210, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
        case 2: {
          for (int i = 11; i < 15; i++)
             leds[i]= CHSV(45, 150, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
        case 3: {
          for (int i = 16; i < 20; i++)
             leds[i]= CHSV(45, 200, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
        break;
        case 4: {
          for (int i = 21; i < 25; i++)
             leds[i]= CHSV(40, 175, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
        break;
        case 5: {
          for (int i = 26; i < 30; i++)
             leds[i]= CHSV(40, 180, 255);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
        
          break;
        case 6: {
          for (int i = 0; i < 5; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
        case 7: {
          for (int i = 6; i < 10; i++)
             leds[i]= CHSV(0, 0, 0);
             break;
        }
          break;
        case 8: {
          for (int i = 11; i < 15; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
        case 9: {
          for (int i = 16; i < 20; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
         case 10: {
          for (int i = 21; i < 25; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
        case 11: {
          for (int i = 26; i < 30; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis1 = millis();
          chec_time_millis = random(MINTIME,MAXNTIME);
          break;
        }
          break;
    }
  }
  

}
// ****************************** ВСПЫШКИ КАМЕР ******************************
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
/*
byte colArry[NUM_LEDS];
byte saturationArry[NUM_LEDS];
byte brightnesspixArry[NUM_LEDS];

// по сути параметры объекта
#define NUMBEROBJECTS 10 // должно быть четное

int timeToShot[NUMBEROBJECTS]; //время до высттрела от обновления таймера после того как выстрелят все
byte colorObject[NUMBEROBJECTS]; //цвета оъектов
bool directionArry[NUMBEROBJECTS];// направление (куда бежит)
bool alredyShot[NUMBEROBJECTS]; // "уже выстрелил", на кждый объект, чтобы не вылетали повторно.
byte positionArry[NUMBEROBJECTS];//позиция на ленте от 1 до колличества светодиодов на ленте

// еще цвет объекта, но это colArry, его выше объявил 
bool shotAll; // флаг что вылетели все, можно обновлять таймер
*/
// ****************************** ЗВЕЗДНЫЕ ВОЙНЫ ******************************

void laserShot() {

  checkRenderTime = 10;// время отрисовки
  if (millis()-renderTime > checkRenderTime)// ограничение времени отрисовки
    {
      renderTime = millis();
      shotAll = true;
      for (int i = 0; i < NUMBEROBJECTS; i++) //цыкл по обработку парамеров на каждый объект------------------------------------------------------------------главный цыкл
        {
          if (millis() - chec_time_millis > timeToShot[i])// если пора выстрелить или уже выстрелил------------------------------------------------------ели выстрелил/летит
          {
            if(alredyShot[i] == false) //если еще не выстрелил нужно вылетить ----------------------------------------------------------------------еще не выстрелил
            {
              shotAll=false; // флак о том что все долетели опускаем, потому что объект только выстрелил
              alredyShot[i] = true; // флак о том что этот обект выстрелил поднимаем, чтобы с следующим проходом полетел 
              
              if (directionArry[i] == true)//ели навлен направо то позиция будет равна первому диоду--------------------------------------выстрелит вправо
              {
                positionArry[i] = 0; // заполняю позицию в объекте выстрела
                leds[positionArry[i]] = CHSV(colorObject[i], 255, 255); // засвечиваю диод по заполненно позиции
              }
              else // //ели навлен налево то позиция будет равна последниму диоду---------------------------------------------------------выстрелит влево
              {
                positionArry[i] = NUM_LEDS-1; // заполняю позицию в объекте выстрела
                leds[positionArry[i]] = CHSV(colorObject[i], 255, 255); // засвечиваю диод по заполненно позиции
              }
            }
            else // ели уже выстрелил, настало время лететь-------------------------------------------------------------------------------------------уже выстрелил
            {
              if (directionArry[i] == true)//ели навлен направо то позиция будет сменена соответственно а старую позицию нужно погасить-----летит вправо
              {
                if (positionArry[i] == NUM_LEDS-1)// если пуля на последнем диоде (уже горит) -----------------------------------------пуля на последнем диоде
                {
                  leds[positionArry[i]] = CHSV(0, 0, 0); //гашу ее
                  positionArry[i] = 250; // и в позицию ставлю отрицательное "-2", м б понадобиться, что пуля пролетела всю ленту
                  shotAll=false;
                }
                if (positionArry[i] < NUM_LEDS-1)// если пуля еще не хочет полететь за границы ленты-----------------------------------пуля не на последнемдиоде
                {
                  leds[positionArry[i]] = CHSV(0, 0, 0); //гашу предыдущее место (то где была пуля)
                  positionArry[i] = positionArry[i] + 1; // заполняю новое место (где будет пуля)
                  leds[positionArry[i]] = CHSV(colorObject[i], 255, 255); // засвечиваю диод на новом месте
                  shotAll=false;// еси хоть один обект не на последнем диоде, то значт он еще летит и таймер обнулять нельзя
                }
              }
              else // ели навлен налево то позиция будет сменена соответственно---------------------------------------------------------------летит влево
              {
                if (positionArry[i] == 0)// если пуля на первом диоде (уже горит) ------------------------------------------------------пуля на первом диоде
                {
                  leds[positionArry[i]] = CHSV(0, 0, 0); //гашу ее
                  positionArry[i] = -250; // и в позицию ставлю отрицательное "-2", м б понадобиться, что пуля пролетела всю ленту
                  shotAll=false;
                }
                if (positionArry[i] > 0)// если пуля еще не хочет полететь за границы ленты---------------------------------------------пуля не на первом диоде
                {
                  leds[positionArry[i]] = CHSV(0, 0, 0); //гашу предыдущее место (то где была пуля)
                  positionArry[i] = positionArry[i] - 1; // заполняю новое место (где будет пуля)
                  leds[positionArry[i]] = CHSV(colorObject[i], 255, 255); // засвечиваю диод на новом месте
                  shotAll=false;// еси хоть один обект не на первом диоде, то значт он еще летит и таймер обнулять нельзя
                }
              }
            }
          }
          else shotAll=false;// если время лететь не пришло то еще никто не выстреливал
        Serial.print(i);
        Serial.print("-");
        Serial.println(alredyShot[i]);
        }//---------------------------------------------------------------------------------------------------------------------------------------------------конец основного цыкла

       /*
       for (int i = 0; i < NUMBEROBJECTS; i++)
       {
        if (alredyShot[i] == false) shotAll = false;
       }*/
       
       Serial.println("---------------------");
       Serial.println(shotAll);
       Serial.println("---------------------");
       if (shotAll == true)// ели значение осталось правдой, значит все уже прилетели и можно обновить таймер.
       {
        chec_time_millis = millis();
    
        for (int i = 0; i < NUMBEROBJECTS/2; i++) //приспаиваю свойста первой половине объектов
          {
            timeToShot[i] = random(50, 2000); //время до вылета, после обновления таймера, после вылета всех, у каждого объекта оно измениться
            alredyShot[i]=false; // возможно это можно будет убрать, потому что при объявлении и так все лож
            positionArry[i] = 250;
          }
        
          for (int i = NUMBEROBJECTS/2; i < NUMBEROBJECTS; i++) //приспаиваю свойста второй половине объектов
          {
            timeToShot[i] = random(50, 4000); //время до вылета, после обновления таймера, после вылета всех, у каждого объекта оно измениться
            alredyShot[i]=false; // возможно это можно будет убрать, потому что при объявлении и так все лож
            positionArry[i] = -250;
          }
        
          
       }
       /*
       Serial.println("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
       Serial.println(shotAll);
       Serial.println("-------------");
            Serial.println(timeToShot[1]);
            Serial.println(directionArry[1]);
            Serial.println(alredyShot[1]);
            Serial.println(colorObject[1]);
            Serial.println(positionArry[1]);
            delay(2000);*/
    }

    
}
// ****************************** ОГОНЬ ******************************
void fire () {
  
}
// ****************************** ВОДА ******************************
void waterDrop () {
  
}
// ****************************** КОМЕТЫ ******************************
void comets () {
  if (millis()-timermillis1 > 50){
    int i = 
  }
  
}
// ****************************** САЛКИ ЦВЕТОВ ******************************
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
