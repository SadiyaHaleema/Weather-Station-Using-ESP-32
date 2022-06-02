/*Wifi Client library 
The zip file was not available on github so I added the code by copying it from github to include the WifiClient library*/
/*
  Client.h - Base class that provides Client
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _WIFICLIENT_H_
#define _WIFICLIENT_H_


#include "Arduino.h"
#include "Client.h"
#include <memory>

class WiFiClientSocketHandle;
class WiFiClientRxBuffer;

class ESPLwIPClient : public Client
{
public:
        virtual int connect(IPAddress ip, uint16_t port, int32_t timeout) = 0;
        virtual int connect(const char *host, uint16_t port, int32_t timeout) = 0;
        virtual int setTimeout(uint32_t seconds) = 0;
};

class WiFiClient : public ESPLwIPClient
{
protected:
    std::shared_ptr<WiFiClientSocketHandle> clientSocketHandle;
    std::shared_ptr<WiFiClientRxBuffer> _rxBuffer;
    bool _connected;
    int _timeout;

public:
    WiFiClient *next;
    WiFiClient();
    WiFiClient(int fd);
    ~WiFiClient();
    int connect(IPAddress ip, uint16_t port);
    int connect(IPAddress ip, uint16_t port, int32_t timeout);
    int connect(const char *host, uint16_t port);
    int connect(const char *host, uint16_t port, int32_t timeout);
    size_t write(uint8_t data);
    size_t write(const uint8_t *buf, size_t size);
    size_t write_P(PGM_P buf, size_t size);
    size_t write(Stream &stream);
    int available();
    int read();
    int read(uint8_t *buf, size_t size);
    int peek();
    void flush();
    void stop();
    uint8_t connected();

    operator bool()
    {
        return connected();
    }
    WiFiClient & operator=(const WiFiClient &other);
    bool operator==(const bool value)
    {
        return bool() == value;
    }
    bool operator!=(const bool value)
    {
        return bool() != value;
    }
    bool operator==(const WiFiClient&);
    bool operator!=(const WiFiClient& rhs)
    {
        return !this->operator==(rhs);
    };

    int fd() const;

    int setSocketOption(int option, char* value, size_t len);
    int setOption(int option, int *value);
    int getOption(int option, int *value);
    int setTimeout(uint32_t seconds);
    int setNoDelay(bool nodelay);
    bool getNoDelay();

    IPAddress remoteIP() const;
    IPAddress remoteIP(int fd) const;
    uint16_t remotePort() const;
    uint16_t remotePort(int fd) const;
    IPAddress localIP() const;
    IPAddress localIP(int fd) const;
    uint16_t localPort() const;
    uint16_t localPort(int fd) const;

    //friend class WiFiServer;
    using Print::write;
};

#endif /* _WIFICLIENT_H_ */


/*Original Code for displaying values for temperature,humidity and rain sensor */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include <DHT.h>

//Connecting with WiFi using network Credentials
const char* ssid     = "Galaxy Note10+ 5G7188";
const char* password = "12345677";

WebServer server(80);



#define DHT11PIN 25               //ESP Pin GPIO25 Connected to DHT sensor
#define rainAnalog 27             //ESP Pin connected to Analog Signal of Rain Sensor 
#define rainDigital 26            //ESP Pin connected to Digital Signal of Rain Sensor

float temp;
int humi;
int rainAnalogVal;
int rainDigitalVal;
DHT dht(DHT11PIN, DHT11);         // for ESP32 Microcontroller



 //  pinMode(rainDigital,INPUT);
 

  void setup()
  {
  //Serial Port for debugging process
    Serial.begin(115200);
    delay(2000);
 /* Start the DHT11 Sensor */
    dht.begin();
  
   /* Connect to WiFi */

    WiFi.begin(ssid, password);
      
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting to ");
    Serial.print(ssid);
      }

      Serial.println("Connected to the WiFi network");
      Serial.print("IP address:");
      Serial.println(WiFi.localIP());
  
    server.on("/",[](){

    const char index_html[] PROGMEM = R"rawliteral(

    <!DOCTYPE html>
    <html>
    <head>
    <title>Weather Station Using ESP 32</title>
    </head>
    <style>

    * {
      margin: 0;
      padding: 0;
    }

    body {
      background: #544947;
      font-family: Montserrat, Arial, sans-serif;
    }
    h2 {
      font-size: 14px;
    }
    .widget {
      box-shadow: 0 40px 10px 5px rgba(0, 0, 0, 0.4);
      margin: 100px auto;
      height: 330px;
      position: relative;
      width: 500px;
    }

    .upper {
      border-radius: 5px 5px 0 0;
      background: #f5f5f5;
      height: 200px;
      padding: 20px;
    }

    .date {
      font-size: 40px;
    }
    .year {
      font-size: 30px;
      color: #c1c1c1;
    }
    .place {
      color: #222;
      font-size: 40px;
    }
    .lower {
      background: #00a8a9;
      border-radius: 0 0 5px 5px;
      font-family: "Advent Pro";
      font-weight: 200;
      height: 130px;
      width: 100%;
    }
    .clock {
      background: #00a8a9;
      border-radius: 100%;
      box-shadow: 0 0 0 15px #f5f5f5, 0 10px 10px 5px rgba(0, 0, 0, 0.3);
      height: 150px;
      position: absolute;
      right: 25px;
      top: -35px;
      width: 150px;
    }

    .hour {
      background: #f5f5f5;
      height: 50px;
      left: 50%;
      position: absolute;
      top: 25px;
      width: 4px;
    }

    .min {
      background: #f5f5f5;
      height: 65px;
      left: 50%;
      position: absolute;
      top: 10px;
      transform: rotate(100deg);
      width: 4px;
    }

    .min,
    .hour {
      border-radius: 5px;
      transform-origin: bottom center;
      transition: all 0.5s linear;
    }

    .infos {
      list-style: none;
    }
    .info {
      color: #fff;
      float: left;
      height: 100%;
      padding-top: 10px;
      text-align: center;
      width: 25%;
    }
    .info span {
      display: inline-block;
      font-size: 40px;
      margin-top: 20px;
    }
    .weather p {
      font-size: 20px;
      padding: 10px 0;
    }
    .anim {
      animation: fade 0.8s linear;
    }

    @keyframes fade {
      0% {
        opacity: 0;
      }
      100% {
        opacity: 1;
      }
    }

    a {
      text-align: center;
      text-decoration: none;
      color: white;
      font-size: 15px;
      font-weight: 500;
    }
  </style>
  <body>
    <div class="widget">
      <div class="clock">
        <div class="min" id="min"></div>
        <div class="hour" id="hour"></div>
      </div>
      <div class="upper">
        <div class="date" id="date"></div>
        <script>
          document.getElementById("date").innerHTML = Date();
        </script>
        <div class="year">Temperature</div>
        <div class="place update" id="temperature"></div>
      </div>
      <div style="text-align: center">
        <a href="#" style="align-items: center"> Weather Station</a>
      </div>
      <div class="lower">
        <ul class="infos">
          <li class="info temp">
            <h2 class="title">TEMPERATURE</h2>
            <span class="update" id="temp">32 &deg;C</span>
          </li>

          <li class="info wind">
            <h2 class="title">RAIN</h2>
            <span class="update" id="rain">266</span>
          </li>
          <li class="info humidity">
            <h2 class="title">HUMIDITY</h2>
            <span class="update" id="humidity">21%</span>
          </li>
        </ul>
      </div>
    </div>

    <script>
      setInterval(drawClock, 2000);

      function drawClock() {
        var now = new Date();
        var hour = now.getHours();
        var minute = now.getMinutes();
        var second = now.getSeconds();

        //Date
        var options = { year: "numeric", month: "long", day: "numeric" };
        var today = new Date();
        document.getElementById("date").innerHTML = today.toLocaleDateString(
          "en-US",
          options
        );

        //hour
        var hourAngle = 360 * (hour / 12) + (360 / 12) * (minute / 60);
        var minAngle = 360 * (minute / 60);
        document.getElementById("hour").style.transform =
          "rotate(" + hourAngle + "deg)";
        //minute
        document.getElementById("min").style.transform =
          "rotate(" + minAngle + "deg)";
      }


     
      document.getElementById("temp").innerHTML = readDHTTemperature();
      document.getElementById("humi").innerHTML = readHumidityTemperature();

      document.getElementById("rain").innerHTML = readRain();


    </script>
  </body>
</html>)rawliteral";
server.send(200,"text/html",index_html);
});

server.begin();
Serial.println("Web Server Started");
  
 }
void loop()
{

 readDHTTemperature();
 readDHTHumidity();
 readRain(); 
 
}




char readDHTTemperature()
  {
       temp = dht.readTemperature();
      if (isnan(temp))
      {    
      Serial.println("Failed to read from DHT11 sensor!");
      }
       else
       {
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print("ºC ");
        Serial.print("\t");

        return char(temp);

       }
        server.handleClient();
        delay(1500);
  }

  char readDHTHumidity()
  {
       humi = dht.readHumidity();
      if (isnan(humi)) 
      {
      Serial.println("Failed to read from DHT11 sensor!");
      }
      else
      {
        Serial.print("Humidity: ");
        Serial.print(humi);
        Serial.print("\t");

        return char(humi);
        
      }
      
      server.handleClient();
      delay(1500);
  }

  char readRain()
  {

       rainAnalogVal = analogRead(rainAnalog);
       rainDigitalVal = digitalRead(rainDigital);
       Serial.print("Rain: ");

       Serial.print(rainDigitalVal);

       Serial.print("\t");
       server.handleClient();
       delay(2000);
       return char(rainDigitalVal);


  }
