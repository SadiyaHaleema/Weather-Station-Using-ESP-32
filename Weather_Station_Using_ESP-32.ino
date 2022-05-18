//#include "DHT.h"
//#define DHT11PIN 16
//
//DHT dht(DHT11PIN, DHT11);
//void setup()
//{
//  
//  Serial.begin(115200);
///* Start the DHT11 Sensor */
//  dht.begin();
//}
//
//void loop()
//{
//  float humi = dht.readHumidity();
//  float temp = dht.readTemperature();
//  Serial.print("Temperature: ");
//  Serial.print(temp);
//  Serial.print("ºC ");
//  Serial.print("Humidity: ");
//  Serial.println(humi);
//  delay(1000);
//}






// for ESP32 Microcontroller
#define rainAnalog 35
#define rainDigital 34

void setup() {
  Serial.begin(9600);
  pinMode(rainDigital,INPUT);
}
void loop() {
  int rainAnalogVal = analogRead(rainAnalog);
  int rainDigitalVal = digitalRead(rainDigital);

  Serial.print(rainAnalogVal);
  Serial.print("\t");
  Serial.println(rainDigitalVal);
  delay(200);
}
