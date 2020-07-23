#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Adafruit_STMPE610.h> // touch sensor

// ESP8266
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#define BOXSIZE 40 
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

TS_Point p;

void drawButton(){
  tft.fillRect(BOXSIZE, BOXSIZE, BOXSIZE*4, BOXSIZE*2, ILI9341_WHITE);
  tft.setCursor(BOXSIZE+20, BOXSIZE*2-5);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLACK);
  tft.println("TOUCH HERE");
}

bool flag = false; 

void drawPress(int x, int y){
  tft.setCursor(BOXSIZE, BOXSIZE*5.5);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_RED);
    Serial.println("\nDetected");
    tft.println("DON'T TOUCH ME!");
    tft.printf("\n    X: %d | Y: %d", x, y);
    drawButton();
    
    delay(1000);
    tft.setCursor(BOXSIZE, BOXSIZE*5.5);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK);
    Serial.println("\nDetected");
    tft.println("DON'T TOUCH ME!");
    tft.printf("\n    X: %d | Y: %d", x, y);
    yield();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //init
  if(!ts.begin()){
    Serial.print("Error: STMPE Error");
  }

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  drawButton();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Fetch touch point from buffer
  TS_Point p = ts.getPoint();

  //Tell the spot we touch
  if(p.x< TS_MINX && p.x>TS_MAXX && p.y<TS_MAXY && p.y>TS_MINY) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("\nX = "); Serial.print(p.x);
  Serial.print("\nY = "); Serial.print(p.y);
  Serial.print("\nPressure = "); Serial.print(p.z);  
  }

  //if we press the "TOUCH HERE"
  if(p.x< BOXSIZE*5 && p.x>BOXSIZE && p.y<BOXSIZE*3 && p.y>BOXSIZE){
    flag = true;
  }

  if(flag){
    drawPress(p.x, p.y);
    flag = false;
  }
  delay(50);
}
