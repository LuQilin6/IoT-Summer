#include <SPI.h>
#include "ILI9341_SPI.h"
#include <MiniGrafx.h>
#include "fonts.h"
#include "Webp.h"

//pins
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2
#define TFT_LED 5

//custom color palette
#define BLACK_COLOR 0x0000
#define WHITE_COLOR 0xFFFF
#define BLUE_COLOR  0x1395
#define PURPLE_COLOR 0xF817

int BITS_PER_PIXEL = 2; //2^2 colors
uint16_t palette[] = {
                      BLACK_COLOR,  //0
                      WHITE_COLOR,  //1
                      BLUE_COLOR,   //2
                      PURPLE_COLOR, //3 
                      };

//Screen dn gfx library initialization
ILI9341_SPI tft = ILI9341_SPI(TFT_CS, TFT_DC);
MiniGrafx gfx = MiniGrafx(&tft, BITS_PER_PIXEL, palette);

void displayTFT(){
  gfx.fillBuffer(0);
  gfx.setColor(2);
  gfx.drawLine(10, 20, 140, 230);
  gfx.drawCircle(120, 160, 120);
  gfx.setFont(Roboto_Mono_Light_16);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 30, "Hello Joe");
  gfx.drawPalettedBitmapFromPgm(20, 100, weatherImage);
  gfx.commit();
}
void setup() {
  // put your setup code here, to run once:
  //power on the screen
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  gfx.init();
  gfx.fillBuffer(0);
  gfx.commit();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  displayTFT();
}
