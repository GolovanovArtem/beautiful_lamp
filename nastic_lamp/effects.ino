#define TRACK_STEP 50



// ****************************** СВЕТЛЯЧКИ ******************************
#define MAX_SPEED 30
#define BUGS_AMOUNT 3
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
byte hue;
void colors() {
  hue += 2;
  CRGB thisColor = CHSV(hue, 255, 255);
  fillAll(CHSV(hue, 255, 255));
}


// ****************************** РАДУГА ******************************
void rainbow() {
  hue += 2;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);
}

// ****************************** КОНФЕТТИ ******************************
void sparkles() {
  byte thisNum = random(0, NUM_LEDS);
  if (getPixColor(thisNum) == 0)
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
  fade();
}

// ****************************** ОГОНЬ ******************************
#define COOLING  55
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
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
void color_red(){
  for (int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CRGB::Red;
  }
}

void color_aqua(){
  for (int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CRGB::DeepSkyBlue;
  }
}

void color_pink(){
  for (int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CRGB::DeepPink;
  }
}

void the_end()
{
   for (int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CRGB::Black;
  }

  if (millis()-timer > time_timer + 1000)
  {
  power.sleep(SLEEP_FOREVER);
  }
}

void fire_1(){
  gPal = CRGBPalette16( CRGB::Black, CRGB::Green, CRGB::White);
  random16_add_entropy( random());
  Fire2012WithPalette(); // run simulation frame, using palette colors
}

void fire_2() {
  gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  random16_add_entropy( random());
  Fire2012WithPalette(); // run simulation frame, using palette colors
}


void fire_3(){
  gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
  random16_add_entropy( random());
  Fire2012WithPalette(); // run simulation frame, using palette colors
}

void fire_4() {
  gPal = CRGBPalette16( CRGB::Black, CRGB::Orange, CRGB::Red,  CRGB::Yellow);
  random16_add_entropy( random());
  Fire2012WithPalette(); // run simulation frame, using palette colors
}

void tab_for_efect(){
  if (millis()-timer < 500)
  {
    for (int i = 0; i < NUM_LEDS; i++) 
    {
    FastLED.setBrightness(200);
    leds[i] = CRGB::Red;
    } 
  } 

  else 
  {
    thisMode = saveMode;
    FastLED.setBrightness(BRIGHTNESS);
  }

}

// ****************** СЛУЖЕБНЫЕ ФУНКЦИИ *******************
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);

    /*// измеряяем цвет текущего пикселя
      uint32_t thisColor = getPixColor(i);

      // если 0, то пропускаем действия и переходим к следующему
      if (thisColor == 0) continue;

      // разбиваем цвет на составляющие RGB
      byte rgb[3];
      rgb[0] = (thisColor >> 16) & 0xff;
      rgb[1] = (thisColor >> 8) & 0xff;
      rgb[2] = thisColor & 0xff;

      // ищем максимум
      byte maximum = max(max(rgb[0], rgb[1]), rgb[2]);
      float coef = 0;

      // если есть возможность уменьшить
      if (maximum >= TRACK_STEP)
      // уменьшаем и находим коэффициент уменьшения
      coef = (float)(maximum - TRACK_STEP) / maximum;

      // далее все цвета умножаем на этот коэффициент
      for (int i = 0; i < 3; i++) {
      if (rgb[i] > 0) rgb[i] = (float)rgb[i] * coef;
      else rgb[i] = 0;
      }
      leds[i] = CRGB(rgb[0], rgb[1], rgb[2]);*/
  }
}
