#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h> //touch sensor
#include "photo.h"
#include "BigPhoto.h"

//ESP8266
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2


#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);


TS_Point p;

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
  
  ts.begin();
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
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  TS_Point p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
  if(p.x< PHOTO_WIDTH&& p.x>0 && p.y<240 && p.y>50){
    tft.setRotation(0);
    Serial.println("detected");
    tft.setCursor(0,0);
    tft.drawRGBBitmap(0 , 0, bigphoto, BIGPHOTO_WIDTH, BIGPHOTO_HEIGHT);
    
  }
  delay(500);
}
