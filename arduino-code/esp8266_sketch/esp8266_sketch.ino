#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>

SoftwareSerial soft_serial(1, 3);


WiFiClient wifi_client;

const char* ssid = "arduinotcc";
const char* password =  "EtecPouso142";

//const String server_url = "http://192.168.1.125/web-gestao-main/";
const String server_url = "http://192.168.1.19/web-gestao-main/";
const String auth_url = server_url + "actions/fetch/arduino_access.php";
const String send_url = server_url + "actions/fetch/arduino_send.php";

const String key = "71bff9bd7d44d5b48f201d6e0129035ebbb912127bc7d6361577c13f68147ad2";
const int id = 1;
const int device = 0;
String token = "0";

// In miliseconds
long upd_time = 3 * 1000;
long last_time = 0;

void keepConnectedToWifi(){
  Serial.println("Reconnecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    yield();
  }
  Serial.println("WiFi CONNECTED");
}

void authenticate(){
  bool connected = false;
  while (!connected){
    Serial.println("Trying to authenticate...");
    HTTPClient http;
    http.begin(wifi_client, auth_url);
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("{\"key\": \""+key+"\", \"id\": "+id+", \"device\": "+device+"}");
    if(httpResponseCode>0) {
        String response = http.getString();
        if (response == "0"){
          Serial.println("Authentication DENIED");
        }
        else {
          connected = true;
          token = response;
          Serial.println("Authentication SUCCESS");
        }
    }
    else {
      Serial.println("Authentication FAILED");
      keepConnectedToWifi();
    }
    http.end();
    delay(100);
    yield();
  }
}

void sendData(int data){
  bool sended = false;
  while (!sended){
    Serial.println("Trying to send data...");
    HTTPClient http;
    http.begin(wifi_client, send_url);
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("{\"token\": \""+token+"\", \"id\": "+id+", \"device\": "+device+", \"medida\": "+data+"}");
    if(httpResponseCode>0) {
        String response = http.getString();
        Serial.println(response);
        if (response == "0"){
          Serial.println("Data sending DENIED");
          authenticate();
        }
        else {
          sended = true;
          Serial.println("Data sending SUCCESS");
        }
    }
    else {
      Serial.println("Data sending FAILED");
      keepConnectedToWifi();
    }
    http.end();
    delay(100);
    yield();
  }
}

void setup(){
  Serial.begin(9600);
  soft_serial.begin(9600);

  Serial.println();
  Serial.println("Booted");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  keepConnectedToWifi();

  authenticate();
}

// The loop function is called in an endless loop
void loop(){
  unsigned long before_time = millis();

  unsigned long data = 0;
  if (soft_serial.available()>0){
    data = soft_serial.read();
  }
  
  Serial.print("Sending data ");
  Serial.println(data);
  sendData(data);

  unsigned long actual_time = millis();
  if (actual_time < before_time){
    before_time = 0;
  }
  unsigned long elapsed_time = actual_time-before_time;
  delay(upd_time-elapsed_time);
}
