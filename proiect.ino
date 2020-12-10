#include <Adafruit_NeoPixel.h>

#define PINPIXEL 4   // input pin Neopixel is attached to

#define PINBUTTON 2 // Button reader

#define NUMPIXELS      24 // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PINPIXEL, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay in milliseconds

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

int buttonState = 0;

void setup() {
  // Initialize NeoPixel 
  pixels.begin();
  // initialize button
  pinMode(PINBUTTON, INPUT);
  
  Serial.begin(9600);
  
}

void loop() {
  setColor();
  
  buttonState = digitalRead(PINBUTTON);
  Serial.println(buttonState);

  if( buttonState = 1) {
    for (int i=0; i < NUMPIXELS; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));

      // This sends the updated pixel color to the hardware.
      pixels.show();
      

      // Delay for a period of time (in milliseconds).
      delay(delayval);
    }
  }else{
      for (int i=0; i < NUMPIXELS; i++) {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      
  
      // This sends the updated pixel color to the hardware.
      pixels.show();

      // Delay for a period of time (in milliseconds).
      delay(delayval);
    }
  }
  
}

// setColor()
// picks random values to set for RGB
void setColor(){
  redColor = random(0, 255);
  greenColor = random(0,255);
  blueColor = random(0, 255);
}
