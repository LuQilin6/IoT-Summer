#include <DHT.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_STMPE610.h> 


// ESP8266
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#define box 40
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750
#define LED_PIN 14

TS_Point p;
bool flag;
bool LEDstatus;
bool isLED;

void drawButton(char *string, int x, int y){
  tft.drawRect(x, y, box*3, box*1.5, ILI9341_WHITE);
  tft.setTextColor(ILI9341_MAGENTA);
  tft.setTextSize(2);
  tft.setCursor(x+box*1.1, y+box*0.35);
  tft.printf(string);
  yield();
}

void drawWelcome(){
  //clear screen
  tft.fillScreen(ILI9341_BLACK);
  //Welcome text
  tft.setTextColor(ILI9341_MAGENTA);
  tft.setTextSize(3);
  tft.setCursor(box*1.5, box);
  tft.printf("WELCOME");
  //LED Button
  drawButton("DHT", box*1.5, box*3);
  drawButton("LED", box*1.5, box*5);
  yield();
}
void drawLED(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(box*1, box);
  tft.printf("LED Control");
  drawButton("1/0", box*1.5, box*3);
  drawButton("Back", box*1.5, box*5);
  yield();
}

void drawDHT(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(box*1, box);
  tft.printf("DHT Control");
  drawButton("1/0", box*1.5, box*3);
  drawButton("Back", box*1.5, box*5);
  yield();
}
void welcomeScreen(){
  drawWelcome();
  flag = true;
}

void dhtScreen(){
  Serial.println("Detect DHT");
  flag=false;
  isLED=false;
  drawDHT();
}

void ledScreen(){
  Serial.println("Detect LED");
  flag=false;
  isLED = true;
  drawLED();
}

void Detect(){
  TS_Point p = ts.getPoint();
  if(p.x< TS_MINX && p.x>TS_MAXX && p.y<TS_MAXY && p.y>TS_MINY){
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("\nX = "); Serial.print(p.x);
  Serial.print("\nY = "); Serial.print(p.y);
  Serial.print("\nPressure = "); Serial.print(p.z);
  if(flag==true){//welcome page
    //go to DHT
    if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*3 &&p.y< box*4.5){
      delay(300);
      if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*3 &&p.y< box*4.5){
        dhtScreen();
        delay(300);
      }
    }    
    //Go to LED
    if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*5 &&p.y< box*6.5){
      delay(300);
      if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*5 &&p.y< box*6.5){
        ledScreen();
        delay(300);
      }
    } 
   }else{//LED and DHT page     
        //LED button
      if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*3 &&p.y< box*4.5){
        delay(300);
        if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*3 &&p.y< box*4.5){
          if(isLED){ //Identify LED/DHT
           LEDstatus = true;
           Serial.print("LED Toggle");
           digitalWrite(LED_PIN, !(digitalRead(LED_PIN));
           delay(300);
          }else{  //This is DHT
            Serial.print("DHT Toggle");
            delay(300);
          }
        }
      }
    //back
      if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*5 &&p.y< box*6.5){
        delay(300);
        if(p.x> box*1.5 &&p.x< box*4.5 &&p.y> box*5 &&p.y< box*6.5){
          welcomeScreen();
          delay(300);
        }
     }
    }
  }
  delay(300);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ts.begin();
  tft.begin();
  pinMode(LED_PIN, OUTPUT);
  welcomeScreen();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Detect();
  delay(300);
}
