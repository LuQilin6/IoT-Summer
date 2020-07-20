#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "photo.h"

#ifdef ESP8266
   #define STMPE_CS 16
   #define TFT_CS   0
   #define TFT_DC   15
   #define SD_CS    2
#endif
#ifdef ESP32
   #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
   #define SD_CS    14
#endif
#ifdef TEENSYDUINO
   #define TFT_DC   10
   #define TFT_CS   4
   #define STMPE_CS 3
   #define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
   #define TFT_DC   PB4
   #define TFT_CS   PA15
   #define STMPE_CS PC7
   #define SD_CS    PC5
#endif
#ifdef ARDUINO_NRF52832_FEATHER /* BSP 0.6.5 and higher! */
   #define TFT_DC   11
   #define TFT_CS   31
   #define STMPE_CS 30
   #define SD_CS    27
#endif
#if defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
   #define TFT_DC   P5_4
   #define TFT_CS   P5_3
   #define STMPE_CS P3_3
   #define SD_CS    P3_2
#endif

// Anything else!
#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__) || \
    defined(ARDUINO_SAMD_ZERO) || defined(__SAMD51__) || defined(__SAM3X8E__) || defined(ARDUINO_NRF52840_FEATHER)
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5
#endif


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

unsigned long Background() {
  tft.fillScreen(ILI9341_WHITE);
  yield();
}

unsigned long SchoolName(){
  tft.setCursor(30, 0);
  tft.setTextColor(ILI9341_WHITE);    tft.setTextSize(2);
  tft.println("La Salle Institute");
  tft.setCursor(45, 30);
  tft.setTextColor(ILI9341_WHITE);    tft.setTextSize(1);
  tft.println("174 William Rd, Troy NY 12180");
}

unsigned long titleBackground() {
  tft.fillRect(0, 0, 280, 50, ILI9341_BLUE);
  yield();
}

unsigned long myInform(){
  tft.setCursor(110, 70);
  tft.setTextColor(ILI9341_BLACK);    tft.setTextSize(3);
  tft.println("Xiaotian Gu");
  tft.setCursor(110, 105);
  tft.setTextColor(ILI9341_BLACK);    tft.setTextSize(2);
  tft.println("XXX St");
  tft.setCursor(110, 140);
  tft.setTextColor(ILI9341_BLACK);    tft.setTextSize(2);
  tft.println("Rensselaer, NY");
}

void IDbackground(){
  tft.fillRect(110, 180, 210, 60, ILI9341_BLUE);
  yield();
}

unsigned long schoolID(){
  tft.setCursor(110, 190);
  tft.setTextColor(ILI9341_YELLOW);    tft.setTextSize(2);
  tft.println(" 2019-2020");
  tft.setCursor(110, 220);
  tft.setTextColor(ILI9341_WHITE);    tft.setTextSize(2);
  tft.println(" ID:XXXXXXXX");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(10);
  Serial.println("FeatherWing TFT Test!");

  tft.begin();

  tft.setRotation(1);
  Background();
  titleBackground();
  IDbackground();
  schoolID();
  SchoolName();
  myInform();
  tft.setCursor(0,50);  
  tft.drawRGBBitmap(0 , 50,myphoto, PHOTO_WIDTH, PHOTO_HEIGHT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
