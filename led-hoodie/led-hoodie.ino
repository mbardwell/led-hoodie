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
#include <Adafruit_NeoMatrix.h>

constexpr int16_t GPIO_LED_ARM_L = 1;  // PIN 1 on v0.1.0 schematics
constexpr int16_t GPIO_LED_ARM_R = 4;  // PIN 2 on v0.1.0 schematics
constexpr int16_t GPIO_LED_BCK_1 = 2;  // PIN 4 on v0.1.0 schematics
constexpr int16_t GPIO_LED_BCK_2 = 5;  // PIN 5 on v0.1.0 schematics
constexpr int16_t GPIO_LED_EAR_1 = 0;  // PIN 6 on v0.1.0 schematics
constexpr int16_t GPIO_LED_EAR_2 = 3;  // PIN 7 on v0.1.0 schematics
constexpr int16_t N_ARM_PIXELS = 60;
constexpr int16_t N_EAR_PIXELS = 16;
constexpr int16_t N_BCK_MATRIX_HGT = 8;
constexpr int16_t N_BCK_MATRIX_WDT = 32;

Adafruit_NeoPixel led_arm_l(N_ARM_PIXELS, GPIO_LED_ARM_L);
Adafruit_NeoPixel led_arm_r(N_ARM_PIXELS, GPIO_LED_ARM_R);
Adafruit_NeoMatrix led_bck_1(N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_1, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS);
Adafruit_NeoMatrix led_bck_2(N_BCK_MATRIX_WDT, N_BCK_MATRIX_HGT, GPIO_LED_BCK_2, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS);
Adafruit_NeoPixel led_ear_1(N_EAR_PIXELS, GPIO_LED_EAR_1);
Adafruit_NeoPixel led_ear_2(N_EAR_PIXELS, GPIO_LED_EAR_2);

constexpr int RETURN_SUCCESS = 0;
int thread_onboard_led_handler();
int thread_arm_handler();
int thread_ear_handler();
int thread_bck_handler();

void setup() {
  Serial.println("LED Hoodie");

  pinMode(LED_BUILTIN, OUTPUT);
  led_arm_l.begin();
  led_arm_r.begin();
  led_bck_1.begin();
  led_bck_1.setTextWrap(false);
  led_bck_1.setTextColor(led_bck_1.Color(0, 0, 150));
  led_bck_2.begin();
  led_bck_2.setTextWrap(false);
  led_bck_2.setTextColor(led_bck_2.Color(150, 150, 0));
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
  thread_onboard_led_handler();
  thread_arm_handler();
  thread_ear_handler();
  thread_bck_handler();
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

int thread_arm_handler() {
  static uint32_t last_time = 250;
  if (millis() - last_time < 1000) {
    return RETURN_SUCCESS;
  }

  led_arm_l.clear();
  led_arm_r.clear();
  for(int i=0; i<N_ARM_PIXELS; i++) {
    led_arm_l.setPixelColor(i, led_arm_l.Color(0, 150, 0));
    led_arm_r.setPixelColor(i, led_arm_r.Color(150, 0, 0));

    led_arm_l.show();
    led_arm_r.show();
    delay(50);
  }

  last_time = millis();
  return RETURN_SUCCESS;
}

int thread_ear_handler() {
  static uint32_t last_time = 750;
  if (millis() - last_time < 1000) {
    return RETURN_SUCCESS;
  }

  led_ear_1.clear();
  led_ear_2.clear();

  for(int i=0; i<N_EAR_PIXELS; i++) {
    led_ear_1.setPixelColor(i, led_ear_1.Color(150, 0, 150));
    led_ear_2.setPixelColor(i, led_ear_2.Color(0, 150, 150));

    led_ear_1.show();
    led_ear_2.show();
    delay(50);
  }

  last_time = millis();
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
