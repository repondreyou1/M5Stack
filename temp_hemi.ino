#include <M5Stack.h>
#include "Adafruit_Si7021.h"

Adafruit_Si7021 sensor = Adafruit_Si7021();

void setup(){
    M5.begin();

    if(!sensor.begin()){
       Serial.println("Did not find Si7021 sensor!");
       while (true);{
           delay(0);
       }
    }
    
    M5.Lcd.setTextSize(3);
}   
 
void loop(){
  float temp = sensor.readTemperature();
  float humid = sensor.readHumidity();

  M5.Lcd.setCursor(40,80);
  M5.Lcd.print("Temp: ");
  M5.Lcd.print(temp, 1);
  M5.Lcd.print("'C");
  M5.Lcd.setCursor(40, 140);
  M5.Lcd.print("Humid: ");
  M5.Lcd.print(humid, 1);
  M5.Lcd.print("%");
  delay(1000);
} 
