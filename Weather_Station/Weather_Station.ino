//Preprocessor includes
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "NTPClientUpdated.h"
#include <OpenWeatherMapCurrent.h>
#include <OpenWeatherMapForecast.h>

//preprocessor defines
#define WIFI_SSID "鹿先森的iPhone吖"
#define WIFI_PASS "gxt200212"
#define MAX_FORECAST 3

//Define time constraints
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -4 * 3600; //-4 hours UTC offset in NY
NTPClientUpdated timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//Initializations
String timeStamp;
String dateStamp;

//weather settings
String OPEN_WEATHER_MAP_LANGUAGE = "en";
String OPEN_WEATHER_MAP_APPID = "f39a39c97227a6fb13a575cdfa754c04";
String OPEN_WEATHER_MAP_LOCATION = "Rensselaer,NY,US";
const boolean IS_METRIC = false;

//Weather objects
OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapForecastData forecasts[MAX_FORECAST];

void connectWiFi(){
  if(WiFi.status() == WL_CONNECTED) return;
  Serial.println(WIFI_SSID);
  Serial.print("/");
  Serial.print(WIFI_PASS);
  Serial.println("Connecting to the WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to the Wifi!");
}

void updateWeatherData(){
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

  delay(100);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
   if(millis() % 3600000 == 1500){
    updateWeatherData();
   }
   
   
   timeStamp = timeClient.getFormattedTime();
   dateStamp = timeClient.getFormattedDate();

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
