#define BLYNK_PRINT Serial
#include "/libs/blynk/blynk.h"
#include "/neopixel/neopixel.h"

///////////////// Blynk Token ////////////////////
char auth[] = "af71bc3cd6e7450cb8eca8b83574870f";
//////////////////////////////////////////////////

///////////////// NeoPixel setup /////////////////
#define PIXEL_PIN D2
#define PIXEL_COUNT 16
#define PIXEL_TYPE WS2811
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
//////////////////////////////////////////////////

int ledPin = A5;
int brightness = 0;

void setup()
{
    Serial.begin(9600);
    delay(5000); // Allow board to settle
    Blynk.begin(auth);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    pinMode(ledPin, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(A0, OUTPUT);
}

// Attach a Button widget (mode: Push) to the Virtual pin 1 - and send sweet tweets!
BLYNK_WRITE(V1) {
    if (param.asInt() == 1) { // On button down...
        // Tweeting!
        // Note:
        //   We allow 1 tweet per minute for now.
        //   Twitter doesn't allow identical subsequent messages.
        Blynk.tweet("My Particle project is tweeting using @blynk_app and it’s awesome!\n @Particle #IoT #blynk");

        // Pushing notification to the app!
        // Note:
        //   We allow 1 notification per minute for now.
        Blynk.notify("You pressed the button and I know it ;)");
    }
}

// Attach a ZeRGBa widget (mode: Merge) to the Virtual pin 2 - and control the built-in RGB led!
BLYNK_WRITE(V2) {
    int r = param[0].asInt();
    int g = param[1].asInt();
    int b = param[2].asInt();
    if (r > 0 && g > 0 && b > 0) {
        RGB.control(true);
        RGB.color(r, g, b);
    } else {
        RGB.control(false);
    }
}

void loop()
{
    Blynk.run();

    if (ModeBtnPressed()) {
        Blynk.notify("Mode button was pressed");
    }

    RGB.color(255, 255, 255);
digitalWrite(ledPin, HIGH);
digitalWrite(D7, LOW);
for(int i = 0; i <= 20; i++) {
    brightness = i * 25;
    analogWrite(A0, brightness);
    delay(200);
}
}

// *** Utility functions

bool ModeBtnPressed() {
    if(millis() > 5000) {
        if(BUTTON_GetDebouncedTime(BUTTON1) >= 50) {
            BUTTON_ResetDebouncedState(BUTTON1);
            rainbow(20);
            return 1;
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////
////////////////////////// NEOPIXEL /////////////////////////
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
////////////////////////////////////////////////////////////
