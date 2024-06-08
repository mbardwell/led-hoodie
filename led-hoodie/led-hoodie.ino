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

constexpr int16_t GPIO_LED_ARM_L = 0;
constexpr int16_t GPIO_LED_ARM_R = 1;
constexpr int16_t GPIO_LED_BCK_1 = 2;
constexpr int16_t GPIO_LED_BCK_2 = 3;
constexpr int16_t GPIO_LED_EAR_1 = 4;
constexpr int16_t GPIO_LED_EAR_2 = 5;

#define NUMPIXELS 16

Adafruit_NeoPixel led_arm_l(NUMPIXELS, GPIO_LED_ARM_L);
Adafruit_NeoPixel led_arm_r(NUMPIXELS, GPIO_LED_ARM_R);
Adafruit_NeoPixel led_bck_1(NUMPIXELS, GPIO_LED_BCK_1);
Adafruit_NeoPixel led_bck_2(NUMPIXELS, GPIO_LED_BCK_2);
Adafruit_NeoPixel led_ear_1(NUMPIXELS, GPIO_LED_EAR_1);
Adafruit_NeoPixel led_ear_2(NUMPIXELS, GPIO_LED_EAR_2);

#define DELAYVAL 500

void setup() {
  led_arm_l.begin();
  led_arm_r.begin();
  led_bck_1.begin();
  led_bck_2.begin();
  led_ear_1.begin();
  led_ear_2.begin();

  led_arm_l.setBrightness(10);
  led_arm_r.setBrightness(10);
  led_bck_1.setBrightness(10);
  led_bck_2.setBrightness(10);
  led_ear_1.setBrightness(10);
  led_ear_2.setBrightness(10);
}

void loop() {
  led_arm_l.clear();
  led_arm_r.clear();
  led_bck_1.clear();
  led_bck_2.clear();
  led_ear_1.clear();
  led_ear_2.clear();

  for(int i=0; i<NUMPIXELS; i++) {
    led_arm_l.setPixelColor(i, led_arm_l.Color(0, 150, 0));
    led_arm_r.setPixelColor(i, led_arm_r.Color(150, 0, 0));
    led_bck_1.setPixelColor(i, led_bck_1.Color(0, 0, 150));
    led_bck_2.setPixelColor(i, led_bck_2.Color(150, 150, 0));
    led_ear_1.setPixelColor(i, led_ear_1.Color(150, 0, 150));
    led_ear_2.setPixelColor(i, led_ear_2.Color(0, 150, 150));

    led_arm_l.show();
    led_arm_r.show();
    led_bck_1.show();
    led_bck_2.show();
    led_ear_1.show();
    led_ear_2.show();

    delay(DELAYVAL);
  }
}
