//Written By: Aarni Junkkala
#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    124   //Kahdessa metrissä 124 lediä
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB ledBuffer[4];
#define UPDATES_PER_SECOND 500

int SwitchMode = 1; //Later should work with a switch

//Mode 0 -> Off

//Mode 1 -> RandomBlink
float WaitBlink = 0.03333333333;
int BlinkAmount = 124;
CRGB BlinkColor = CRGB(0,0,0);
//Mode 2 -> Hue all colors
int num = 0;

//Mode 3 -> Reacts to playing
int lastTime = 0; //Shared with Mode 4
int TriggerVoltage = 500;
//Mode 4 -> LightShow
//Shares lastTime variable from Mode 3

void setup() {
  BlinkColor = Hue(240);
  Serial.begin(9600);
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  StartTest();
  delay(1000);  
}

void StartTest(){
  for (int i = 0; i < NUM_LEDS; i++){
    ledBuffer[i] = CRGB(255,255,255);
    FastLED.show();
    delay(250 / NUM_LEDS);
  }
  for (int i = 0; i < NUM_LEDS; i++){
    ledBuffer[i] = CRGB(0,0,0);
    FastLED.show();
    delay(250 / NUM_LEDS);
  }
}

void FillBuffer() {
  ledBuffer[0] = CRGB(255, 255, 255);
  ledBuffer[1] = CRGB(170, 170, 170);
  ledBuffer[2] = CRGB(85, 85, 85);
  ledBuffer[3] = CRGB(0, 0, 0);
}

void loop()
{
  if(SwitchMode == 0){
    SetAllLedColor(CRGB(0,0,0));
  }

  
  if(SwitchMode == 1){
    for(int i = 0; i < BlinkAmount; i++){
      SetRandomLedColor(BlinkColor);
    }
    delay(WaitBlink);
    for(int i = 0; i < BlinkAmount; i++){
      SetRandomLedColor(CRGB(0,0,0));
    }
    delay(WaitBlink);
  }

  if(SwitchMode == 2){
    SetAllLedColor(Hue(num));
    num += 1;
    if (num >= 360){
      num = 0;
      //SetAllLedColor(CRGB(0,0,0));
      //delay(1000);
    }
    delay(2000 / 360);
  }

  //LightShow
  if(SwitchMode == 4){
    MoveLeds();

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    float aika = 0.1;
    int CurrentTime = round(millis() / (1000 * aika));
    if(CurrentTime != lastTime){
      lastTime = CurrentTime;
      FillBuffer();
    }
  }

  //Reacting to guitar Input
  if(SwitchMode == 3){
    MoveLeds();

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    float aika = 0.1;
    int CurrentTime = round(millis() / (1000 * aika));
    int val = analogRead(A0);
    Serial.println(val - TriggerVoltage);
    if(val > TriggerVoltage){
      if(CurrentTime != lastTime){
        lastTime = CurrentTime;
        FillBuffer();
      }
    }
  }
  
}

void MoveLeds()
{
  for ( int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }

  leds[0] = ledBuffer[0];
  for (int i = 0; i < 3; i++) {
    ledBuffer[i] = ledBuffer[i + 1];
  }
}

void SetAllLedColor(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = color;
  }
  FastLED.show();
}

void SetRandomLedColor(CRGB color){
  leds[random(0,NUM_LEDS)] = color;
  FastLED.show();
}

CRGB Hue(int val){
 val = val % 360;
 int red,blu,gre = 0; //Default to zero

 // -- Green -- //
 //Raise and lower
 if(abs(120 - val) > 60 && abs(120 - val) < 120){
  gre = (60 - abs(120 - val)) * 4.25;
 }
 //Solid color
 if(val >= 60 && val <= 180){
  gre = 255;
 }
 //No color
 if(val >= 240){
  gre = 0;
 }
 
 // -- Blue -- //
 //Raise and lower
 if(abs(240 - val) > 60 && abs(240 - val) < 120){
  blu = (120 - abs(240 - val)) * 4.25;
 }
 //Solid color
 if(val >= 180 && val <= 300){
  blu = 255;
 }
 //No color
 if(val <= 120)
  {
    blu = 0;
  }

 // -- Red -- //
 //Raise and lower
 int HolderVal = val; //To take care of rotating around 360
 if(HolderVal > 240){
  HolderVal -= 360;
 }
 if(abs(0 - HolderVal) > 60 && abs(0 - HolderVal) < 120){
  red = (120 - abs(0 - HolderVal)) * 4.25;
 }
 //61 = 59
 //119 = 1
 //Solid color
 if(val <= 60 || val >= 300){
  red = 255;
 }
 if(val >= 120 && val <= 240){
  red = 0;
 }
 return CRGB(red,gre,blu);
}
