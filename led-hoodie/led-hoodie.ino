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
 *
 * Ideas:
 *   Pacman https://projecthub.arduino.cc/giobbino/arduino-nano-pac-man-7d5f47
 *   Mouth using eyes and top back panel
*/

#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "pins_arduino.h"
#include "led-colors.hpp"
#include "led-designs.hpp"

constexpr int16_t GPIO_LED_ARM_L = 1;     // PIN 1 on v0.1.0 schematics is GP0
constexpr int16_t GPIO_LED_ARM_R = 4;     // PIN 2 on v0.1.0 schematics is GP1
constexpr int16_t GPIO_LED_BCK_T = 2;     // PIN 4 on v0.1.0 schematics is GP2
constexpr int16_t GPIO_LED_BCK_B = 5;     // PIN 5 on v0.1.0 schematics is GP3
constexpr int16_t GPIO_LED_EAR_L = 0;     // PIN 6 on v0.1.0 schematics is GP4
constexpr int16_t GPIO_LED_EAR_R = 3;     // PIN 7 on v0.1.0 schematics is GP5
constexpr int16_t GPIO_LED_ONBOARD = 22;  // LED at the bottom of the PCB
constexpr int16_t GPIO_MICROPHONE = 26;
constexpr int16_t N_ARM_PIXELS = 30;
constexpr int16_t N_EAR_PIXELS = 16;
constexpr int16_t N_BCK_MATRIX_HGT = 8;
constexpr int16_t N_BCK_MATRIX_WDT = 32;
constexpr uint8_t BRIGHTNESS = 25;

CRGB led_arm_l_buf[N_ARM_PIXELS];
CRGB led_arm_r_buf[N_ARM_PIXELS];
CRGB led_ear_l_buf[N_EAR_PIXELS];
CRGB led_ear_r_buf[N_EAR_PIXELS];

Adafruit_NeoMatrix led_bck_top(N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_T, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
Adafruit_NeoMatrix led_bck_bot(N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_B, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

constexpr int RETURN_SUCCESS = 0;
int thread_onboard_led_handler();
int setup_arm();
int thread_arm_handler();
int setup_ear();
int thread_ear_handler();
int setup_bck();
int thread_bck_handler_scroll();
int thread_bck_handler_pop();
int thread_microphone_handler();

enum Mode {
  ITS_PARTY_TIME,
  I_LOVE_YOU,
  MOUTH,
};

Mode mode;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_LED_ONBOARD, OUTPUT);
  digitalWrite(GPIO_LED_ONBOARD, LOW);
  setup_arm();
  setup_ear();
  setup_bck();
  led_bck_top.setBrightness(BRIGHTNESS);
  led_bck_bot.setBrightness(BRIGHTNESS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  static uint32_t last_time = 0;

  if (millis() - last_time > 60000) {
    mode = static_cast<Mode>(static_cast<int>(mode) + 1);
    last_time = millis();
  }

  switch (mode) {
    case ITS_PARTY_TIME:
      thread_microphone_handler();
      thread_onboard_led_handler();
      thread_arm_handler();
      thread_ear_handler();
      thread_bck_handler_scroll();
      break;
    case I_LOVE_YOU:
      thread_microphone_handler();
      thread_onboard_led_handler();
      thread_arm_handler();
      thread_ear_handler();
      thread_bck_handler_pop();
      break;
    default:
      mode = ITS_PARTY_TIME;
      break;
  }

  FastLED.show();
}

int thread_onboard_led_handler() {
  static uint32_t last_time = 0;
  if (millis() - last_time < 1000) {
    return RETURN_SUCCESS;
  }

  digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == HIGH ? LOW : HIGH);
  // digitalWrite(GPIO_LED_ONBOARD, digitalRead(GPIO_LED_ONBOARD) == HIGH ? LOW : HIGH);

  last_time = millis();
  return RETURN_SUCCESS;
}

int thread_microphone_handler() {
  static uint32_t last_time = 0;
  if (millis() - last_time < 333) {
    return RETURN_SUCCESS;
  }
  int _min = 1024;
  int _max = 0;
  int sensor_value = 0;
  int delta = 0;

  for (int i = 0; i < 250; ++i) {
    sensor_value = analogRead(GPIO_MICROPHONE);
    _min = min(sensor_value, _min);
    _max = max(sensor_value, _max);
  }

  delta = _max - _min;
  // Serial.print("Min: ");
  // Serial.print(_min);
  // Serial.print(" Max: ");
  // Serial.print(_max);
  // Serial.print(" Delta: ");
  // Serial.println(delta);

  if (delta > 100)
    digitalWrite(GPIO_LED_ONBOARD, digitalRead(GPIO_LED_ONBOARD) == HIGH ? LOW : HIGH);

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
  } else {
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
  led_bck_top.begin();
  led_bck_top.setTextWrap(false);
  led_bck_top.setTextColor(LED_CYAN_HIGH);

  led_bck_bot.begin();
  led_bck_bot.setTextWrap(false);
  led_bck_bot.setTextColor(LED_ORANGE_HIGH);

  led_bck_top.fillScreen(LED_WHITE_LOW);
  led_bck_top.show();
  led_bck_bot.fillScreen(LED_WHITE_LOW);
  led_bck_bot.show();
  delay(1000);
  led_bck_top.clear();
  led_bck_top.show();
  led_bck_bot.clear();
  led_bck_bot.show();

  return RETURN_SUCCESS;
}

int thread_bck_handler_scroll() {
  static int x = led_bck_top.width();
  static uint32_t last_time = 0;
  if (millis() - last_time < 10) {
    return RETURN_SUCCESS;
  }
  char buf[] = "IT'S PARTY TIME";

  led_bck_top.clear();
  led_bck_top.show();
  led_bck_bot.clear();
  led_bck_bot.show();

  led_bck_top.setCursor(x, 0);
  led_bck_top.write(buf);
  led_bck_top.show();
  led_bck_bot.setCursor(x, 0);
  led_bck_bot.write(buf);
  led_bck_bot.show();

  if (--x < -100)
    x = led_bck_top.width();

  last_time = millis();
  return RETURN_SUCCESS;
}

int thread_bck_handler_pop() {
  static int x = led_bck_top.width();
  static uint32_t last_time = 0;
  static uint32_t pass = 0;
  if (millis() - last_time < 1000) {
    return RETURN_SUCCESS;
  }

  led_bck_top.clear();
  led_bck_top.show();
  led_bck_bot.clear();
  led_bck_bot.show();
  led_bck_top.setTextColor(LED_RED_HIGH);
  led_bck_bot.setTextColor(LED_RED_HIGH);

  switch (pass) {
    case 1:
      led_bck_top.setCursor((led_bck_top.width() / 2) - 2, 0);
      led_bck_top.write("I");
      led_bck_bot.setCursor((led_bck_bot.width() / 2) - 2, 0);
      led_bck_bot.write("I");
      break;
    case 2:
      led_bck_top.setCursor((led_bck_top.width() / 2) - 11, 0);
      led_bck_top.write("LOVE");
      led_bck_bot.setCursor((led_bck_bot.width() / 2) - 11, 0);
      led_bck_bot.write("LOVE");
      break;
    case 3:
      led_bck_top.setCursor((led_bck_top.width() / 2) - 8, 0);
      led_bck_top.write("YOU");
      led_bck_bot.setCursor((led_bck_bot.width() / 2) - 8, 0);
      led_bck_bot.write("YOU");
      break;
    case 4:
      led_bck_top.drawBitmap((led_bck_top.width() / 2) - 3, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
      led_bck_top.drawBitmap(1, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
      led_bck_top.drawBitmap(led_bck_top.width() - 8, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
      led_bck_bot.drawBitmap((led_bck_top.width() / 2) - 3, 0, led_designs::bitmap_heart_7_x_7, 7, 7, LED_RED_HIGH);
      break;
    default:
      pass = 0;
      break;
  }
  pass += 1;
  led_bck_top.show();
  led_bck_bot.show();

  last_time = millis();
  return RETURN_SUCCESS;
}
