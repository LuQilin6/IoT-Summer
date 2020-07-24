//Preprocessor includes
#include "ILI9341_SPI.h"
#include <MiniGrafx.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "NTPClientUpdated.h"
#include <OpenWeatherMapCurrent.h>
#include <OpenWeatherMapForecast.h>
#include "Weather_logo.h"
#include "font.h"  
//Orbitron_Medium_16 || Yellowtail_Regular_24 || Schoolbell_Regular_22
//Calligraffitti_Regular_24 ||Calligraffitti_Regular_45 || Calligraffitti_Regular_40 || Calligraffitti_Regular_30
#include "weathericons.h"

//preprocessor defines
#define WIFI_SSID "鹿先森的iPhone吖"
#define WIFI_PASS "gxt200212"
#define MAX_FORECAST 3

#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2
#define TFT_LED  5

//Palette colors
#define BLACK_COLOR 0x0000
#define WHITE_COLOR 0xFFFF
#define BLUE_COLOR  0x7E3C


//Define time constraints
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -4 * 3600; //-4 hours UTC offset in NY
NTPClientUpdated timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//Initializations
String timeStamp;
String dateStamp;
int screen =1;
boolean isDebug = false;

//weather settings
String OPEN_WEATHER_MAP_LANGUAGE = "en";
String OPEN_WEATHER_MAP_APPID = "f39a39c97227a6fb13a575cdfa754c04";
String OPEN_WEATHER_MAP_LOCATION = "Rensselaer,NY,US";
const boolean IS_METRIC = false;

//Weather objects
OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapForecastData forecasts[MAX_FORECAST];

//Graphics lib initialization
int BITS_PER_PIXEL =2; //2^2 = 4 colors
uint16_t palette[] = {
  BLACK_COLOR,
  WHITE_COLOR,
  ILI9341_YELLOW,
  BLUE_COLOR,
};

ILI9341_SPI tft =ILI9341_SPI(TFT_CS, TFT_DC);
MiniGrafx gfx = MiniGrafx(&tft, BITS_PER_PIXEL, palette);

void connectWiFi(){
  if(WiFi.status() == WL_CONNECTED) {
    return;
  }
  Serial.println(WIFI_SSID);
  Serial.print("/");
  Serial.print(WIFI_PASS);
  Serial.println("Connecting to the WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int i=0;
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    if(i>90){
      i=0;
    }
    i = i+10;
    drawProgress(i, "Connecting to Wifi (" + String(WIFI_SSID) + ")");
    Serial.print(".");
  }
  drawProgress(100, "Connected to Wifi (" + String(WIFI_SSID) + ")");
  Serial.println("Connected to the Wifi!");
}

void updateWeatherData(){
  drawProgress(50, "Updating Current Weather");
  OpenWeatherMapCurrent *currentWeatherClient = new OpenWeatherMapCurrent();
  currentWeatherClient->setMetric(IS_METRIC);
  currentWeatherClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient->updateCurrent(&currentWeather, OPEN_WEATHER_MAP_APPID, OPEN_WEATHER_MAP_LOCATION);
  delete currentWeatherClient;
  currentWeatherClient = nullptr;

  //Forecast Weather
  OpenWeatherMapForecast * forecastClient = new OpenWeatherMapForecast();
  forecastClient->setMetric(IS_METRIC);
  forecastClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  forecastClient->updateForecasts(forecasts, OPEN_WEATHER_MAP_APPID, OPEN_WEATHER_MAP_LOCATION, MAX_FORECAST);
  delete forecastClient;
  forecastClient = nullptr;

  drawProgress(70, "Updating Current Weather");
  delay(100);
}

void welcomeScreen(){
  gfx.fillBuffer(0);
  gfx.setFont(Orbitron_Medium_16);
  gfx.setColor(1);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 30, "WEATHER STATION");
  gfx.drawPalettedBitmapFromPgm(60, 60, Weather_icon);
  gfx.setFont(Yellowtail_Regular_24);
  gfx.setColor(1);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 290, "X n  GU");
  gfx.commit();
}

void drawProgress(int percentage, String text){
  gfx.fillBuffer(0);
  gfx.setFont(Orbitron_Medium_16);
  gfx.setColor(1);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 30, "WEATHER STATION");
  gfx.drawPalettedBitmapFromPgm(60, 60, Weather_icon);

  //progressbars
  gfx.setColor(3);
  gfx.drawRect(10, 198, 220, 15);
  gfx.setColor(3);
  gfx.fillRect(12, 200, 216 *percentage/100, 11);
  
  //text for more info
  gfx.setColor(1);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setFont(Orbitron_Medium_16);
  gfx.drawString(120, 230, text);
  
  gfx.setFont(Yellowtail_Regular_24);
  gfx.setColor(1);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(120, 290, "X  GU");
  gfx.commit(); 
}

void weatherScreen(){
  gfx.fillBuffer(0);

  //time at y=20
  gfx.setFont(Calligraffitti_Regular_30);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(120, 10, timeStamp);

  //Date at y=55
  gfx.setFont(Calligraffitti_Regular_24);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(120, 55, dateStamp);

  //Display temperature
  gfx.setFont(Calligraffitti_Regular_40);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(180, 95, String(int(round(currentWeather.temp))) + "F");
  //Display description
  gfx.setFont(Calligraffitti_Regular_24);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(180, 130, String(currentWeather.description));
  
  //Display the weather icon
  gfx.drawPalettedBitmapFromPgm(10, 80, getMeteoconIconFromProgmem(currentWeather.icon));

  //city name
  gfx.setFont(Calligraffitti_Regular_24);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(120, 175, String(currentWeather.cityName));

  //temperature in 3 hours
  gfx.setFont(Calligraffitti_Regular_24);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(45, 200, "3 hours");
  gfx.drawString(36, 280, String(int(round(forecasts[0].temp))) + "F");

   //icon in 3 hour
  gfx.drawPalettedBitmapFromPgm(15, 230, getMiniMeteoconIconFromProgmem(forecasts[0].icon));

  //temperature in 6 hours
  gfx.setFont(Calligraffitti_Regular_24);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(120, 200, "6 hours");
  gfx.drawString(111, 280, String(int(round(forecasts[1].temp))) + "F");

   //icon in 6 hour
  gfx.drawPalettedBitmapFromPgm(90, 230, getMiniMeteoconIconFromProgmem(forecasts[1].icon));

  //temperature in 9 hours
  gfx.setFont(Calligraffitti_Regular_24);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(1);
  gfx.drawString(195, 200, "9 hours");
  gfx.drawString(186, 280, String(int(round(forecasts[2].temp))) + "F");

   //icon in 9 hour
  gfx.drawPalettedBitmapFromPgm(165, 230, getMiniMeteoconIconFromProgmem(forecasts[2].icon));

  
   gfx.commit();
}

void todaysInformation(){
  gfx.fillBuffer(0);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  
  gfx.setFont(Schoolbell_Regular_22);
  gfx.setColor(1);
  gfx.drawString(10, 10, "Temperature(Min): " + String(int(round(currentWeather.tempMin))) + "F");

  gfx.setFont(Schoolbell_Regular_22);
  gfx.setColor(1);
  gfx.drawString(10, 40, "Temperature(Max): " + String(int(round(currentWeather.tempMax))) + "F");

  gfx.setFont(Schoolbell_Regular_22);
  gfx.setColor(1);
  gfx.drawString(10, 70, "Humidity: " + String(int(round(currentWeather.humidity))) + "%");

  gfx.setFont(Schoolbell_Regular_22);
  gfx.setColor(1);
  gfx.drawString(10, 90, "Pressure: " + String(int(round(currentWeather.pressure))) + " mb");

  gfx.setFont(Schoolbell_Regular_22);
  gfx.setColor(1);
  gfx.drawString(10, 130, "Sunrise: " + String(timeClient.getFormattedTime(true, currentWeather.sunrise + utcOffsetInSeconds)));
  
  gfx.setFont(Schoolbell_Regular_22);
  gfx.setColor(1);
  gfx.drawString(10, 160, "Sunset: " + String(timeClient.getFormattedTime(true, currentWeather.sunset + utcOffsetInSeconds)));

  gfx.commit();
}
//-------------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  gfx.init();
  welcomeScreen();
  Serial.println("\n\n\n");
  connectWiFi();
  updateWeatherData();
  timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
 
   while(!timeClient.forceUpdate()){
      timeClient.forceUpdate();
   }
   if(millis() % 3600000 < 1500){
    updateWeatherData();
   }
   if(millis()% 10000 < 240){
    if(screen == 1){
      screen =2;
    }else if(screen == 2){
      screen =1; 
    }
   }
   timeStamp = timeClient.getFormattedTime();
   dateStamp = timeClient.getFormattedDate();

   if (screen ==1) {
    weatherScreen();
   }else if(screen == 2){
    todaysInformation();
   }
   
   if(isDebug){
     Serial.println("Time: " +timeStamp);
     Serial.println("Date: " + dateStamp);
     Serial.println("Temp: " + String(int(round(currentWeather.temp))) + "ºF");
     Serial.print("3 hour forecast: ");
     Serial.println("Temp: " + String(int(round(forecasts[0].temp))) + "ºF");
     Serial.print("6 hour forecast: ");
     Serial.println("Temp: " + String(int(round(forecasts[1].temp))) + "ºF");
     Serial.print("9 hour forecast: ");
     Serial.println("Temp: " + String(int(round(forecasts[2].temp)))+ "ºF");
     Serial.println("-------------------------------------------------");
     delay(1000);
    }
   delay(10);
}
