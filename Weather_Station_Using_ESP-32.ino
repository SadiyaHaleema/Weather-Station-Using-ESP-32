//#include <WiFiClient.h>
#include <WiFi.h>
const char* ssid     = "Hostels Wi-Fi";
const char* password = "";
//set web server port 80
WiFiServer server(80);
#include "DHT.h"
#define DHT11PIN 16
#define rainAnalog 35
#define rainDigital 34
DHT dht(DHT11PIN, DHT11);
// for ESP32 Microcontroller

void setup()
{
  
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());

/* Start the DHT11 Sensor */
  dht.begin();
   pinMode(rainDigital,INPUT);
}

void loop()
{

  if((WiFi.status()==WL_CONNECTED))
  {
      Serial.println("You can try to Ping me");
      delay(5000);

  }
  else
  {
      Serial.println("Connection Lost");

  }


  
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Humidity: ");
  Serial.println(humi);
  delay(1000);

  int rainAnalogVal = analogRead(rainAnalog);
  int rainDigitalVal = digitalRead(rainDigital);

  Serial.print(rainAnalogVal);
  Serial.print("\t");
  Serial.println(rainDigitalVal);
  delay(200);
}
