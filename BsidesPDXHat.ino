#BsidesPDX Hat source code.
#Built up from Adafruit's Neopixel library and the strandtest.pde example

#include <Adafruit_NeoPixel.h>
#include <Keypad.h>

#define PIN 10
#define LENGTH 37
#define MAXLED 31

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, PIN, NEO_GRB + NEO_KHZ800);


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 9,8,7,6 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 5,4,3,2, };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

uint32_t buffer[LENGTH];
uint32_t backup[LENGTH];
int offset=0;
boolean rotateOn=false;
int rotateSpeed=100;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}

void colorChase(uint32_t c){
  for (int i=0;i<10;i++){
    colorWipe(c, 5);
    colorWipe(strip.Color(0, 0, 0), 5); // clear
  }
}

void push(uint32_t c){
  buffer[offset]=c;//Red
  offset++;
  offset%=LENGTH;
  show();
}

void rotate()
{
  delay(rotateSpeed);
  offset++;
  offset%=LENGTH;
  show();
}   

void fadeOut(int d=5)
{
  uint8_t r,g,b;
  for(int i=0; i<LENGTH; i++) backup[i]=buffer[i];  

  for(int j=0;j<=MAXLED;j++){
    for(int i=0; i<LENGTH; i++){
      r = (uint8_t)(buffer[i] >> 16),
      g = (uint8_t)(buffer[i] >>  8),
      b = (uint8_t)buffer[i];
 
      if (r>0)r--;
      if (g>0)g--;
      if (b>0)b--;
      
      buffer[i]=((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
    }
    delay(d);
    show();
  }
}

void fadeIn(int d=1)
{
  uint8_t r,g,b;
  uint8_t R,G,B;
  for(int j=0;j<=MAXLED;j++){
    for(int i=0; i<LENGTH; i++){

      R = (uint8_t)(backup[i] >> 16),
      G = (uint8_t)(backup[i] >>  8),
      B = (uint8_t)backup[i];

      r = (uint8_t)(buffer[i] >> 16),
      g = (uint8_t)(buffer[i] >>  8),
      b = (uint8_t)buffer[i];
 
      if (r<R)r++;
      if (g<G)g++;
      if (b<B)b++;
      buffer[i]=((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
    }
    delay(d);
    show();
  }
}

void show() {
  for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, buffer[(i+offset)%LENGTH]);
    }
  strip.show();
}



void loop() {  
  char customKey = customKeypad.getKey();
  switch (customKey){
    case '1':
      push(strip.Color(MAXLED,0,0));
      break;  
    case '2':
      push(strip.Color(0,MAXLED,0));
      break;  
    case '3':
      push(strip.Color(0,0,MAXLED));
      break;  
    case 'A':
      push(strip.Color(0,0,0));
      break;  
    case '4':
      push(strip.Color(MAXLED/2,MAXLED/2,0));
      break;  
    case '5':
      push(strip.Color(0,MAXLED/2,MAXLED/2));
      break;  
    case '6':
      push(strip.Color(MAXLED/2,0,MAXLED/2));
      break;  
    case 'B':
      push(strip.Color(MAXLED/3,MAXLED/3,MAXLED/3));
      break;
    case 'C':
      rotateOn=true;
      break;
    case 'D':
      rotateOn=false;
      break;
    case '*':
      fadeOut();
      break;
    case '7':
      fadeIn();
      break;
    case '0':
      if (rotateSpeed>1)rotateSpeed/=2;
      break;
    case '8':
      if (rotateSpeed<1000)rotateSpeed*=2;
      break;
    case '9':
      break;
    case '#':
      break;
  }
  if (rotateOn==true) rotate();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
