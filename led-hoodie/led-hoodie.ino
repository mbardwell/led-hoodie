/**
 * @file led-hoodie.ino
 * @brief LED Hoodie
 * @details This is the main file for the LED Hoodie project.
 * 
 * Based off of NeoPixel Ring simple sketch (c) 2013 Shae Erisson
 * Released under the GPLv3 license to match the rest of the
 * Adafruit NeoPixel library
*/

#include <Adafruit_NeoPixel.h>

#define PIN_LED_ARM_L        0
#define PIN_LED_ARM_R        1
#define PIN_LED_BCK_1        2

#define NUMPIXELS 16

Adafruit_NeoPixel led_arm_l(NUMPIXELS, PIN_LED_ARM_L, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_arm_r(NUMPIXELS, PIN_LED_ARM_R, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_bck_1(NUMPIXELS, PIN_LED_BCK_1, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500

void setup() {
  led_arm_l.begin();
  led_arm_r.begin();
  led_bck_1.begin();

  led_arm_l.setBrightness(10);
  led_arm_r.setBrightness(10);
  led_bck_1.setBrightness(10);
}

void loop() {
  led_arm_l.clear();
  led_arm_r.clear();
  led_bck_1.clear();

  for(int i=0; i<NUMPIXELS; i++) {

    led_arm_l.setPixelColor(i, led_arm_l.Color(0, 150, 0));
    led_arm_r.setPixelColor(i, led_arm_r.Color(0, 150, 0));
    led_bck_1.setPixelColor(i, led_bck_1.Color(0, 150, 0));

    led_arm_l.show();
    led_arm_r.show();
    led_bck_1.show();

    delay(DELAYVAL);
  }
}
