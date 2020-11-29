/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

const long interval = 1000; //Co ile ma wysyłać stan
unsigned long previousMillis = 0;  
unsigned long currentMillis = 0;

//IPAddress local_IP(10, 1, 1, 103); //Statyczny adres ip sterownika //NIE DZIALA
//IPAddress subnet(255, 255, 255, 0);
const String server_adress = "http://10.1.1.100/test/"; //Aktualnie jest serwer 
const String php_target = "insert.php"; //nazwa skryptu uzupelniajacego. Trzeba pamiętać o "?"
const String field_one = "?nazwa=";
const String sensor_id_number = "1"; //Id utworzonego w bazie danych sensora
const String field_two = "&dane=";






void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  
//  if (!WiFi.config(local_IP,subnet)) {
//    Serial.println("STA Failed to configure");
//  }
  WiFi.mode(WIFI_STA);
  
  WiFiMulti.addAP("NumerNaBis", "cA6G64jFP@");

}

void loop() {
  // wait for WiFi connection
    //Generowanie danych
  if ((WiFiMulti.run() == WL_CONNECTED)) {
   currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
      float liczba = random(0, 200)/100.0;
    
    Serial.print("[HTTP] begin... \n"); 
    String complete_adress = server_adress + php_target + field_one + sensor_id_number + field_two + liczba;
    Serial.println(complete_adress);
    Serial.println();
  
    WiFiClient client;

    HTTPClient http;

    
    if (http.begin(client, complete_adress)) {  


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  }

  //delay(10000);
}
