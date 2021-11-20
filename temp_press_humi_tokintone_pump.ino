#include <M5Stack.h>
#include <WiFi.h>
#include "Seeed_BMP280.h"
#include <Wire.h>
#include <driver/adc.h>
#include <HTTPClient.h>

#define INPUT_PIN 36
#define PUMP_PIN 21

BMP280 bmp280;

const char* ssid = "repondreyou1";
const char* password ="19840206";

char json[4096];
HTTPClient http;

bool flag1 = true;
int rawADC;

void setup() {
  adc_power_acquire(); 
  M5.begin();
  if(!bmp280.init()){
       Serial.println("BPM sensor broken!");
       while (true);
  }

  WiFi.begin(ssid,password);
  Serial.print("connecting");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  Serial.println("\nWiFi Connected.");
  Serial.println(WiFi.localIP());

  M5.Lcd.setTextSize(3);
  M5.Lcd.println("WiFi Connected:");

  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextDatum(TC_DATUM);
  pinMode(INPUT_PIN,INPUT);
  pinMode(PUMP_PIN,OUTPUT);
  pinMode(25,OUTPUT);
  digitalWrite(25,0);
}

char info[30];

void loop() {
  float temp = bmp280.getTemperature();
  uint32_t pressure_t = bmp280.getPressure();
  uint32_t pressure = pressure_t /100;

  M5.lcd.fillRect(40, 80, 240, 200, BLACK);

  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(40,80);
  M5.Lcd.printf("Temp: %5.1f'C", temp);
  M5.Lcd.setCursor(40,120);
  M5.Lcd.printf("Preess: %dhPa", pressure);
  
  rawADC = analogRead(INPUT_PIN);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(40, 160);
  M5.Lcd.print("ADC: "+ String(rawADC));
  Serial.print("Watering ADC value: ");
   Serial.println(rawADC);
    if(rawADC < 2500){
      flag1 = true;
      digitalWrite(PUMP_PIN,flag1);
//      delay(6000);
  }else{
      flag1 = false;
      digitalWrite(PUMP_PIN,flag1);      
//      delay(6000);
  }                                                                                            

  sprintf(json,"{\"app\":\"1134\",\"record\":{\"temp\":{\"value\":\"%s\"},\"press\":{\"value\":\"%s\"},\"himi\":{\"value\":\"%s\"}}}",String(temp),String(pressure),String(rawADC));

  int httpResponseCode=0;
  http.begin("https://kojirou.cybozu.com/k/v1/record.json");
  http.addHeader(F("X-Cybozu-API-Token"),F("rL225b8pbinuXzSIZ8iV0AWji04FNtdoXQauRxTJ"));
  http.addHeader(F("Content-type"),F("application/json"));
  httpResponseCode=http.POST(json);
  Serial.printf("httpResponseCode=%d \n",httpResponseCode);
  Serial.println(http.getString());
 
  M5.update();
  delay(30000);
}
