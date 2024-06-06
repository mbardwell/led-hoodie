// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN_LED_ARM_L        0 // On Trinket or Gemma, suggest changing this to 1
#define PIN_LED_ARM_R        1 // On Trinket or Gemma, suggest changing this to 1
#define PIN_LED_BCK_1        2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel led_arm_l(NUMPIXELS, PIN_LED_ARM_L, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_arm_r(NUMPIXELS, PIN_LED_ARM_R, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_bck_1(NUMPIXELS, PIN_LED_BCK_1, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  led_arm_l.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  led_arm_r.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  led_bck_1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  led_arm_l.setBrightness(10);
  led_arm_r.setBrightness(10);
  led_bck_1.setBrightness(10);
}

void loop() {
  led_arm_l.clear(); // Set all pixel colors to 'off'
  led_arm_r.clear(); // Set all pixel colors to 'off'
  led_bck_1.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels1.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    led_arm_l.setPixelColor(i, led_arm_l.Color(0, 150, 0));
    led_arm_r.setPixelColor(i, led_arm_r.Color(0, 150, 0));
    led_bck_1.setPixelColor(i, led_bck_1.Color(0, 150, 0));

    led_arm_l.show();   // Send the updated pixel colors to the hardware.
    led_arm_r.show();   // Send the updated pixel colors to the hardware.
    led_bck_1.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
}
