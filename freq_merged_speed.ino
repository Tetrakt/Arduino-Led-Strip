/* Base build + new Music Reactive Module
 * merged with freq resp build
 * Beta build
 * May function faster on ESP8266
 */
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <stdlib.h> 
#include <arduinoFFT.h>

/*FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif
*/

#define BRIGHTNESS 150
#define FRAMES_PER_SECOND  74

#define NUM_LEDS 150
#define DATA_PIN 3 //LED PIN FOR NEOPIXEL

#define samples 64
#define SAMPLING_FREQUENCY 10000
arduinoFFT FFT = arduinoFFT(); // OBJECT of fft
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[samples]; //create vector of size SAMPLES to hold real values
double vImag[samples]; //create vector of size SAMPLES to hold imaginary values

int c=10;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int i;
int RECV_PIN = 13;//
int states=14;
#define PIN 3 //LED PIN FOR FASTLED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
CRGB leds[NUM_LEDS];

int AudioPin = 0;

int code;//BUTTON VALUE

void setup() 
{
  samplingPeriod = round(10000000 * (1.0 / SAMPLING_FREQUENCY)); //Period in microseconds
   delay(1500); // 1.5 second delay for recovery
  pinMode(RECV_PIN,INPUT); 
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  delay(100);
}

//neopixel Setup code
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

//END of neopixel code

void rainbow() //RAINBOW, GAMER STYLE
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}
//code for theatre chase
byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
void theaterChaseRainbow(int SpeedDelay) 
{
  byte *c;
  
  for (int j=0; j < 256; j++)
  {     // cycle all 256 colors in the wheel
    
    if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
    for (int q=0; q < 3; q++)
    {
      
    if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
        for (int i=0; i < NUM_LEDS; i=i+3) 
        {
          c = Wheel( (i+j) % 255);
          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        
    if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
        showStrip();
       
        delay(SpeedDelay);
       for (int i=0; i < NUM_LEDS; i=i+3) 
          setPixel(i+q, 0,0,0); 
        
    
  }
}

    if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 5);
  int pos = beatsin8( 20, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 32;//to  change speed
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin16( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void ColorStatic(int r,int g, int b)
{
  for(i=0;i<NUM_LEDS;i++)
  leds[i].setRGB(r,g,b);
  FastLED.show();
}


//white base, 4 colors sparkling randomly.
void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay)
{
  
  setAll(red,green,blue);
  
  int Pix1 = random(NUM_LEDS);
  int Pix2 = random(NUM_LEDS);
  int Pix3 = random(NUM_LEDS);
  int Pix4 = random(NUM_LEDS);
      if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(100);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c++;
  return(0);
  }
  }
  setPixel(Pix1,0x00,0xff,0x00);//colors
  setPixel(Pix2,0x00,0x00,0xff);
  setPixel(Pix3,255,51,255);
  setPixel(Pix4,255,0,127);//sparkle colorSnowSparkle(0x10, 0x10, 0x10, 20, random(100,1000));
  showStrip();
      if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(100);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c++;
  return(0);
  }
  }
  delay(SparkleDelay);
  setPixel(Pix1,red,green,blue);
  setPixel(Pix2,red,green,blue);
  setPixel(Pix3,red,green,blue);
  setPixel(Pix4,red,green,blue);
  showStrip();
  delay(SpeedDelay);
    if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(100);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c++;
  return(0);
  }
}
}
 
//complete randomness, sparkle
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(150);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c++;
  return(0);
  }
}
}

//you can modify color in function call/
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(150);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c=0;
  return(0);
  }
}
      Position++; // = 0; 
      //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) 
      {
          if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(150);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c=0;
  return(0);
  }
}
        
        // sine wave, 3 offset waves make a rainbow!
        float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
         
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                  ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      
      showStrip();
      delay(WaveDelay);
  }
  if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(150);
  if(digitalRead(RECV_PIN)==HIGH)
  {
    c=0;
  return(0);
  }
}
}

void bootanim()
{
   for(i=0;i<NUM_LEDS;i++)
  {
    leds[i].setRGB(0,0,0);
    FastLED.show();
  }
  delay(50);
  for(i=0;i<NUM_LEDS;i++)
  {
    leds[i].setHue((255-i)*(1.711));
    FastLED.show();
    delay(10);
  }
delay(250);
 
c=0;
return(0);
  
}


void FreqSetCol(int H,int S)
{
  int HSV_Value[] = {25,100,150,200,255,200,150,100,25 };
  for ( int i=0;i<9;i++)
  {
    leds[0].setHSV(H,S, HSV_Value[i]);
        if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
    FastLED.show();
    for(int j=NUM_LEDS- 1; j>=1;j--)
    {
      leds[j]=leds[j-1];
    }
    
    FastLED.show();

    delay(20); // speed of wave
    
    if(digitalRead(RECV_PIN)== HIGH)
          {
            delay(100);
            if(digitalRead(RECV_PIN)== HIGH)
            {
              c++;
              return(0);
            }
          }
  }
}


void FreqSetCol()
{
  for ( int i=0;i<9;i++)
  {
    leds[0].setHSV(0,0,0);
        if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
    FastLED.show();
    for(int j=NUM_LEDS- 1; j>=1;j--)
    {
      leds[j]=leds[j-1];
    }
    
    FastLED.show();

    delay(20); // speed of wave
    
    if(digitalRead(RECV_PIN)== HIGH)
          {
            delay(100);
            if(digitalRead(RECV_PIN)== HIGH)
            {
              c++;
              return(0);
            }
          }
  }
}

void FreqWave()
{
  int Peak,i;
  while(1)
  {
    for(int j= 0; j < samples; j++)
    {
      vReal[j]=analogRead(0); // read audio input
      vImag[j] = 0;
    }
    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, samples);

  /*  if(analogRead(0)>1) // dont ask why 2
    {
    Peak = FFT.MajorPeak(vReal, samples, SAMPLING_FREQUENCY);
    
    //Peak = (int)random(100,5000);
    }
    else
    Peak=0;
    */
    Peak = FFT.MajorPeak(vReal, samples, SAMPLING_FREQUENCY);
    
    if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }

             if (Peak >= 100 && Peak <= 760)
            FreqSetCol(0,255);
          else if (Peak > 760 && Peak <= 1420)
            FreqSetCol(42,255);
          else if (Peak > 1420 && Peak <= 2080)
            FreqSetCol(85,255);
          else if (Peak > 2080 && Peak <= 2740)
            FreqSetCol(127,255);
          else if (Peak > 2740 && Peak <= 3400)
            FreqSetCol(170,255);
          else if (Peak > 3400 && Peak <= 4060)
            FreqSetCol(190,255);
          else if (Peak > 4060 && Peak <= 4720)
            FreqSetCol(212,255);
          else if(Peak > 4720 && Peak <=5000)
            FreqSetCol(255,255);
          else
          FreqSetCol(); // set black

    if(digitalRead(RECV_PIN)== HIGH)
    {
      delay(100);
      if(digitalRead(RECV_PIN) == HIGH)
      {
        c++;
        return(0);
      }
    }
    
  }
}

void loop() 
{
  //button input.
  if(digitalRead(RECV_PIN)==HIGH)
  {
    delay(200);
    if(digitalRead(RECV_PIN)==HIGH)
    {
      if(c==states)
      c=0;
      else
      c++;
    }
  }


 
  switch (c)
  {
    case 0:
     ColorStatic(255,0,0);
     
    break;
    case 1:
    ColorStatic(0,0,255);
    
    break;
    case 2:
    ColorStatic(0,0255,0);
    
    break;
    case 3:
    ColorStatic(255,0,127);
    
    break;
    case 4:
    ColorStatic(255,51,255);
   
    break;
    case 5:
    ColorStatic(0,255,255);
    
    break;
    case 6:
    ColorStatic(255,255,0);
    
    break;
    case 7:
    rainbow();
    FastLED.show();  
    break;
    
    case 8:
    sinelon();
    FastLED.show();  
    
    break;
    
    case 9:
    bpm();
    FastLED.show();  
    
    break;

    case 10:
    FreqWave();
    
    case 11:
    theaterChaseRainbow(50);
    break;
    
    case 12:
     SnowSparkle(50,50,50, 200, 200); // set first 3, set base color of "snow" 
    break;

    case 13:
    {
      byte LedColor[24][3] = 
{
{255,0,0},{255,128,0},{255,191,0},
{255,255,0},{191,255,0},{128,255,0},
{64,255,0},{0,255,0},{0,255,64},
{0,255,128},{0,255,191},{0,255,255},
{0,191,255},{0,128,255},{0,64,255},
{0,0,255},{64,0,255},{128,0,255},
{191,0,255},{255,0,255},{255,0,191},
{255,0,128},{255,0,64}};

byte ColorChoice = random(0,23);

Sparkle(LedColor[ColorChoice][0],LedColor[ColorChoice][1],LedColor[ColorChoice][2],25);
   // Sparkle(random(255), random(255), random(255), 15); //RGB FORMAT WITH POOR COLORS
    //TO UPDATE
   // int PixelHue = random(0,255);
    //Sparkle(PixelHue,70);
    }
    
    break;
    
    case 14:
    
    RunningLights(0,255,229, 50);//this modifies color

   
    //RunningLights(0xff,0xff,0x00, 5);
    break;
    case -1:
    bootanim();
    break;
    
    
  }
  EVERY_N_MILLISECONDS( 5 ) { gHue++; } // slowly cycle the "base color" through the rainbow,alters speed of rainbow,bpm and sinelon
  }
      

  
