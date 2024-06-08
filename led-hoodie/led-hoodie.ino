/**
 * @file led-hoodie.ino
 * @brief LED Hoodie
 * @details This is the main file for the LED Hoodie project.
 * 
 * Based off of NeoPixel Ring simple sketch (c) 2013 Shae Erisson
 * Released under the GPLv3 license to match the rest of the
 * Adafruit NeoPixel library
*/

#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

constexpr int16_t GPIO_LED_ARM_L = 1;  // PIN 1 on v0.1.0 schematics
constexpr int16_t GPIO_LED_ARM_R = 4;  // PIN 2 on v0.1.0 schematics
constexpr int16_t GPIO_LED_BCK_1 = 2;  // PIN 4 on v0.1.0 schematics
constexpr int16_t GPIO_LED_BCK_2 = 5;  // PIN 5 on v0.1.0 schematics
constexpr int16_t GPIO_LED_EAR_L = 0;  // PIN 6 on v0.1.0 schematics
constexpr int16_t GPIO_LED_EAR_R = 3;  // PIN 7 on v0.1.0 schematics
constexpr int16_t N_ARM_PIXELS = 60;
constexpr int16_t N_EAR_PIXELS = 16;
constexpr int16_t N_BCK_MATRIX_HGT = 8;
constexpr int16_t N_BCK_MATRIX_WDT = 32;

CRGB led_arm_l_buf[N_ARM_PIXELS];
CRGB led_arm_r_buf[N_ARM_PIXELS];
CRGB led_bck_1_buf[N_BCK_MATRIX_WDT* N_BCK_MATRIX_HGT];
CRGB led_bck_2_buf[N_BCK_MATRIX_WDT* N_BCK_MATRIX_HGT];
CRGB led_ear_l_buf[N_EAR_PIXELS];
CRGB led_ear_r_buf[N_EAR_PIXELS];

FastLED_NeoMatrix led_bck_1(led_bck_1_buf, N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_1, NEO_MATRIX_TOP + NEO_MATRIX_LEFT);
FastLED_NeoMatrix led_bck_2(led_bck_2_buf, N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_2, NEO_MATRIX_TOP + NEO_MATRIX_LEFT);

constexpr int RETURN_SUCCESS = 0;
int thread_onboard_led_handler();
int setup_arm();
int thread_arm_handler();
int setup_ear();
int thread_ear_handler();
int setup_bck();
int thread_bck_handler();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  setup_arm();
  setup_ear();
  setup_bck();
}

void loop() {
  thread_onboard_led_handler();
  thread_arm_handler();
  thread_ear_handler();
  thread_bck_handler();

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

  // led_arm_l.setBrightness(10);
  // led_arm_r.setBrightness(10);

  return RETURN_SUCCESS;
}

int thread_arm_handler() {
  static uint32_t last_time = 0;
  static uint32_t i = 0;
  if (millis() - last_time < 100) {
    return RETURN_SUCCESS;
  }

  led_arm_l_buf[i] = CRGB::Red;
  led_arm_r_buf[i] = CRGB::Green;
  if (i > N_ARM_PIXELS) {
    for (int j = 0; j < N_ARM_PIXELS; j++) {
      led_arm_l_buf[j] = CRGB::Black;
      led_arm_r_buf[j] = CRGB::Black;
    }
    i = 0;
  }
  else
   i += 1;

  last_time = millis();
  return RETURN_SUCCESS;
}

int setup_ear() {
  FastLED.addLeds<NEOPIXEL, GPIO_LED_EAR_L>(led_ear_l_buf, N_EAR_PIXELS);
  FastLED.addLeds<NEOPIXEL, GPIO_LED_EAR_R>(led_ear_r_buf, N_EAR_PIXELS);

  // led_ear_1.setBrightness(10);
  // led_ear_2.setBrightness(10);

  return RETURN_SUCCESS;
}

int thread_ear_handler() {
  static uint32_t last_time = 0;
  static uint32_t i = 0;
  if (millis() - last_time < 100) {
    return RETURN_SUCCESS;
  }

  led_ear_l_buf[i] = CRGB::Blue;
  led_ear_r_buf[i] = CRGB::Red;
  if (i > N_EAR_PIXELS) {
    for (int j = 0; j < N_EAR_PIXELS; j++) {
      led_ear_l_buf[j] = CRGB::Black;
      led_ear_r_buf[j] = CRGB::Black;
    }
    i = 0;
  }
  else
   i += 1;

  last_time = millis();
  return RETURN_SUCCESS;
}

int setup_bck() {
  FastLED.addLeds<NEOPIXEL, GPIO_LED_BCK_1>(led_bck_1_buf, N_BCK_MATRIX_WDT* N_BCK_MATRIX_HGT);
  FastLED.addLeds<NEOPIXEL, GPIO_LED_BCK_2>(led_bck_2_buf, N_BCK_MATRIX_WDT* N_BCK_MATRIX_HGT);

  led_bck_1.begin();
  led_bck_1.setTextWrap(false);
  led_bck_1.setTextColor(led_bck_1.Color(0, 0, 150));

  led_bck_2.begin();
  led_bck_2.setTextWrap(false);
  led_bck_2.setTextColor(led_bck_2.Color(0, 150, 0));

  led_bck_1.setBrightness(10);
  led_bck_2.setBrightness(10);

  return RETURN_SUCCESS;
}

int thread_bck_handler() {
  static int x = led_bck_1.width();
  static uint32_t last_time = 0;
  if (millis() - last_time < 100) {
    return RETURN_SUCCESS;
  }

  led_bck_1.fillScreen(0);
  led_bck_2.fillScreen(0);

  led_bck_1.setCursor(x, 0);
  led_bck_2.setCursor(x, 0);

  led_bck_1.print("HELLO");
  led_bck_2.print("HELLO");

  if (--x < -36)
    x = led_bck_1.width();

  led_bck_1.show();
  led_bck_2.show();
  
  delay(50);

  last_time = millis();
  return RETURN_SUCCESS;
}
