#include <M5Stack.h>
#include <WiFi.h>
#include "Seeed_BMP280.h"
#include <Wire.h>
#include <driver/adc.h>
#include <HTTPClient.h>

BMP280 bmp280;

const char* ssid = "repondreyou1";
const char* password ="19840206";

char json[4096];
HTTPClient http;
 
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
}
 
void loop() {
  M5.update();

  float temp = bmp280.getTemperature();
  uint32_t pressure_t = bmp280.getPressure();
  uint32_t pressure = pressure_t /100;
  
  if(M5.BtnA.wasReleased()){
    M5.Lcd.drawJpgFile(SD, "/prof.jpg");
  }
  if(M5.BtnB.wasReleased()){
     M5.Lcd.drawJpgFile(SD, "/QR.jpg");
  }
  
  String message = "会ったよ";

  if(M5.BtnC.wasReleased()){
    sprintf(json,"{\"app\":\"1150\",\"record\":{\"Btn_C\":{\"value\":\"%s\"},\"temp\":{\"value\":\"%s\"},\"press\":{\"value\":\"%s\"}}}",message,String(temp),String(pressure));

    int httpResponseCode=0;
    http.begin("https://kojirou.cybozu.com/k/v1/record.json");
    http.addHeader(F("X-Cybozu-API-Token"),F("7g5zJQ0OK3rR4ymXRkAOzw3XUfOtoF0giLNEX9MI"));
    http.addHeader(F("Content-type"),F("application/json"));
    httpResponseCode=http.POST(json);
    Serial.printf("httpResponseCode=%d \n",httpResponseCode);
    Serial.println(http.getString());
  }
  delay(30);
}
