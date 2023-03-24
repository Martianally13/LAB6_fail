#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"


char * endpoint1 =ENDPOINT1;
char * endpoint2 = ENDPOINT2;
#define FAN 22
#define LIGHT 23

void setup() 
{
  pinMode(FAN, OUTPUT);
  pinMode(LIGHT, OUTPUT);

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
 {
  if(WiFi.status()== WL_CONNECTED)
  {
    
    HTTPClient http;  
    String http_response1;


    http.begin(endpoint1);
    int httpResponseCode1 = http.GET();
   
    if (httpResponseCode1 > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode1);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response1);
      }
      else
      {   
        Serial.print("Error code: ");
        Serial.println(httpResponseCode1);
      } 

      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, http_response);

      if (error) 
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
//--------------------------------------------------------------------------------------------
      http.end();

      http.begin(endpoint2)
      String httpRequestData2;

      int httpResponseCode2 = http.PUT(httpRequestData2);

      String http_response2;

          if (httpResponseCode2 > 0) 
          {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode2); 

            Serial.print("HTTP Response from server: ");
            http_response = http.getString();
            Serial.println(http_response2);

            StaticJsonDocument<48> doc;

            DeserializationError error = deserializeJson(doc, input);

            if (error) 
            {
              Serial.print("deserializeJson() failed: ");
              Serial.println(error.c_str());
              return;
            }

            float temperature = doc["temperature"]; // 29.2

        }                   
  }
  else 
  {
    return;
  }
}