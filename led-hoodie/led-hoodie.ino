/**
 * @file led-hoodie.ino
 * @brief LED Hoodie
 * @details This is the main file for the LED Hoodie project.
 * 
 * Based off of NeoPixel Ring simple sketch (c) 2013 Shae Erisson
 * Released under the GPLv3 license to match the rest of the
 * Adafruit NeoPixel library
 * 
 * Mixes NeoMatrix and FastLED libraries. 
 * Unfortunately, the FastLED NeoMatrix library was buggy on the PicoW
*/

#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "led-colors.hpp"
#include "led-designs.hpp"

constexpr int16_t GPIO_LED_ARM_L = 1;  // PIN 1 on v0.1.0 schematics
constexpr int16_t GPIO_LED_ARM_R = 4;  // PIN 2 on v0.1.0 schematics
constexpr int16_t GPIO_LED_BCK_1 = 2;  // PIN 4 on v0.1.0 schematics
constexpr int16_t GPIO_LED_BCK_2 = 5;  // PIN 5 on v0.1.0 schematics
constexpr int16_t GPIO_LED_EAR_L = 0;  // PIN 6 on v0.1.0 schematics
constexpr int16_t GPIO_LED_EAR_R = 3;  // PIN 7 on v0.1.0 schematics
constexpr int16_t N_ARM_PIXELS = 30;
constexpr int16_t N_EAR_PIXELS = 16;
constexpr int16_t N_BCK_MATRIX_HGT = 8;
constexpr int16_t N_BCK_MATRIX_WDT = 32;
constexpr uint8_t BRIGHTNESS = 25;

CRGB led_arm_l_buf[N_ARM_PIXELS];
CRGB led_arm_r_buf[N_ARM_PIXELS];
CRGB led_ear_l_buf[N_EAR_PIXELS];
CRGB led_ear_r_buf[N_EAR_PIXELS];

Adafruit_NeoMatrix led_bck_1(N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_1, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
Adafruit_NeoMatrix led_bck_2(N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_2, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

constexpr int RETURN_SUCCESS = 0;
int thread_onboard_led_handler();
int setup_arm();
int thread_arm_handler();
int setup_ear();
int thread_ear_handler();
int setup_bck();
int thread_bck_handler_scroll();
int thread_bck_handler_pop();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  setup_arm();
  setup_ear();
  setup_bck();
  led_bck_1.setBrightness(BRIGHTNESS);
  led_bck_2.setBrightness(BRIGHTNESS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  thread_onboard_led_handler();
  thread_arm_handler();
  thread_ear_handler();
  thread_bck_handler_pop();

  FastLED.show();
}

int thread_onboard_led_handler() {
  static uint32_t last_time = 0;
  if (millis() - last_time < 1000) {
    return RETURN_SUCCESS;
  }

  digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == HIGH ? LOW : HIGH);

  last_time = millis();
  return RETURN_SUCCESS;
}

int setup_arm() {
  FastLED.addLeds<NEOPIXEL, GPIO_LED_ARM_L>(led_arm_l_buf, N_ARM_PIXELS);
  FastLED.addLeds<NEOPIXEL, GPIO_LED_ARM_R>(led_arm_r_buf, N_ARM_PIXELS);

  return RETURN_SUCCESS;
}

int thread_arm_handler() {
  static uint32_t last_time = 0;
  static uint32_t i = 0;
  static uint32_t i_ = 0;
  if (millis() - last_time < 50) {
    return RETURN_SUCCESS;
  }

  if (++i > N_ARM_PIXELS)
    i = 0;

  led_arm_l_buf[i_] = CRGB::Black;
  led_arm_r_buf[i_] = CRGB::Black;
  led_arm_l_buf[i] = CRGB::Red;
  led_arm_r_buf[i] = CRGB::Green;

  i_ = i;
  last_time = millis();
  return RETURN_SUCCESS;
}

int setup_ear() {
  FastLED.addLeds<NEOPIXEL, GPIO_LED_EAR_L>(led_ear_l_buf, N_EAR_PIXELS);
  FastLED.addLeds<NEOPIXEL, GPIO_LED_EAR_R>(led_ear_r_buf, N_EAR_PIXELS);

  return RETURN_SUCCESS;
}

int thread_ear_handler() {
  static uint32_t last_time = 0;
  static uint32_t i = 0;
  static uint32_t i_ = 0;
  static bool up = true;
  if (millis() - last_time < 25) {
    return RETURN_SUCCESS;
  }

  if (up) {
    if (i < N_EAR_PIXELS)
      i += 1;
    else
      up = false;
  } 
  else {
    if (i > 0)
      i -= 1;
    else
      up = true;
  }

  led_ear_l_buf[i_] = CRGB::Black;
  led_ear_r_buf[i_] = CRGB::Black;
  led_ear_l_buf[i] = CRGB::HotPink;
  led_ear_r_buf[i] = CRGB::AliceBlue;

  i_ = i;  
  last_time = millis();
  return RETURN_SUCCESS;
}

int setup_bck() {
  led_bck_1.begin();
  led_bck_1.setTextWrap(false);
  led_bck_1.setTextColor(LED_CYAN_HIGH);

  led_bck_2.begin();
  led_bck_2.setTextWrap(false);
  led_bck_2.setTextColor(LED_ORANGE_HIGH);

  led_bck_1.fillScreen(LED_WHITE_LOW);
  led_bck_1.show();
  led_bck_2.fillScreen(LED_WHITE_LOW);
  led_bck_2.show();
  delay(1000);
  led_bck_1.clear();
  led_bck_1.show();
  led_bck_2.clear();
  led_bck_2.show();

  return RETURN_SUCCESS;
}

int thread_bck_handler_scroll() {
  static int x = led_bck_1.width();
  static uint32_t last_time = 0;
  if (millis() - last_time < 10) {
    return RETURN_SUCCESS;
  }
  char buf[] = "IT'S PARTY TIME";

  led_bck_1.clear();
  led_bck_1.show();
  led_bck_2.clear();
  led_bck_2.show();

  led_bck_1.setCursor(x, 0);
  led_bck_1.write(buf);
  led_bck_1.show();
  led_bck_2.setCursor(x, 0);
  led_bck_2.write(buf);
  led_bck_2.show();

  if (--x < -100)
    x = led_bck_1.width();

  last_time = millis();
  return RETURN_SUCCESS;
}

int thread_bck_handler_pop() {
  static int x = led_bck_1.width();
  static uint32_t last_time = 0;
  static uint32_t pass = 0;
  if (millis() - last_time < 1000) {
    return RETURN_SUCCESS;
  }

  led_bck_1.clear();
  led_bck_1.show();
  led_bck_2.clear();
  led_bck_2.show();
  led_bck_1.setTextColor(LED_RED_HIGH);
  led_bck_2.setTextColor(LED_RED_HIGH);

  switch (pass)
  {
  case 1:
    led_bck_1.setCursor((led_bck_1.width() / 2) - 2, 0);
    led_bck_1.write("I");
    led_bck_2.setCursor((led_bck_2.width() / 2) - 2, 0);
    led_bck_2.write("I");
    break;
  case 2:
    led_bck_1.setCursor((led_bck_1.width() / 2) - 11, 0);
    led_bck_1.write("LOVE");
    led_bck_2.setCursor((led_bck_2.width() / 2) - 11, 0);
    led_bck_2.write("LOVE");
    break;
  case 3:
    led_bck_1.setCursor((led_bck_1.width() / 2) - 8, 0);
    led_bck_1.write("YOU");
    led_bck_2.setCursor((led_bck_2.width() / 2) - 8, 0);
    led_bck_2.write("YOU");
    break;
  case 4:
    led_bck_1.drawBitmap((led_bck_1.width() / 2) - 3, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
    led_bck_1.drawBitmap(1, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
    led_bck_1.drawBitmap(led_bck_1.width() - 8, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
    led_bck_2.drawBitmap((led_bck_1.width() / 2) - 3, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
    break;
  default:
    pass = 0;
    break;
  }
  pass += 1;
  led_bck_1.show();
  led_bck_2.show();

  last_time = millis();
  return RETURN_SUCCESS;
}
