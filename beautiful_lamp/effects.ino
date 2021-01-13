#define TRACK_STEP 50

byte hue;
byte brightnesspix;
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

#define MINTIME 1000
#define MAXNTIME 2000

void condominium(){
  hue = 50;
  if (millis()-timermillis > win_activ_time)
  {
      switch (random(0, 8)) 
    {
        case 0: {
          for (int i = 0; i < 5; i++)
             leds[i]= CHSV(hue, 255, brightness);
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
        }
          break;
        case 1: {
          for (int i = 8; i < 13; i++)
             leds[i]= CHSV(hue, 255, brightness);
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
        }
          break;
        case 2: {
          for (int i = 16; i < 21; i++)
             leds[i]= CHSV(hue, 255, brightness);
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
        }
          break;
        case 3: {
          for (int i = 24; i < 29; i++)
             leds[i]= CHSV(hue, 255, brightness);
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
        }
          break;
        case 4: {
          for (int i = 0; i < 5; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
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
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
        }
          break;
        case 7: {
          for (int i = 24; i < 29; i++)
             leds[i]= CHSV(0, 0, 0);
          timermillis = millis();
          win_activ_time = random(MINTIME,MAXNTIME);
        }
          break;
    }
  }
  

}
void flashescamera() {

  hue += 1;
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV((byte)(hue + i * float(255 / 255)), 255, 70);
  }
  leds[random(0, NUM_LEDS-1)] = CHSV(hue, 255, 255); //возможно стоит мигать полеже
}

// *************** ВИНИГРЕТ ***************
void vinigret() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) {
      
    }
  }
}

// ****************** СЛУЖЕБНЫЕ ФУНКЦИИ *******************
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);
  }
}
