#include <sstream>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

const uint8_t fingerprint[20] = {0x40, 0xaf, 0x00, 0x6b, 0xec, 0x90, 0x22, 0x41, 0x8e, 0xa3, 0xad, 0xfa, 0x1a, 0xe8, 0x25, 0x41, 0x1d, 0x1a, 0x5, 0xb3};

ESP8266WiFiMulti WiFiMulti;

#include "parkings.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

const char *ssid = "OnePlus3";
const char *password = "mot2passe";

String http_dataResult = "";
String http_distResult = "";

const float userLatitude = 43.608398437500008;
const float userLongitude = 3.879869937896729;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  delay(1500);
}

void loop() {
    
  getParkingPosition();
  delay(1000);
  getParkingData();

  parking_t *ptr = parkings;

  while(ptr->id) {
    scoreAssignment(ptr);
    ptr++;
  }

  printBestParking();
  
  Serial.println("Wait 3min before the next round");
  delay(180000);
}

void getParkingData() {
  parking_t *ptr = parkings;
  bool requestFailed = false;

  Serial.println("2. Récupération des informations de chacun des parkings...");

  while(ptr->id) {
    
    if((WiFiMulti.run() == WL_CONNECTED)){
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  
      client->setFingerprint(fingerprint);
      client->setInsecure();
  
     HTTPClient https_data;
     
      if (https_data.begin(*client, buildURL(ptr->id))) {
  
        int http_dataCode = https_data.GET();
  
        if (http_dataCode > 0) {

          // file found at server
          if (http_dataCode == HTTP_CODE_OK || http_dataCode == HTTP_CODE_MOVED_PERMANENTLY) {
            http_dataResult = https_data.getString();
            Serial.printf("\t--> %s : OK\n", ptr->id);
          }
        } else {
          requestFailed = true;
          Serial.printf("[HTTPS] GET... failed, error: %s, failure for : %s\n", https_data.errorToString(http_dataCode).c_str(), ptr->id);
        }
        
        https_data.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }

    delay(2000);

    ptr++;

    if(!requestFailed)
      parseXML(http_dataResult.c_str(), &(ptr-1)->parkingData);
  }
}

void getParkingPosition() {
  parking_t *ptr = parkings;
  bool requestFailed = false;

  Serial.println("1. Récupération de la distance de chacun des parkings...");

  while(ptr->id) {
   
    if((WiFiMulti.run() == WL_CONNECTED)){
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  
      client->setFingerprint(fingerprint);
      client->setInsecure();
  
     HTTPClient https_dist;

      if (https_dist.begin(*client, distanceBetweenTwoPointsURL(userLatitude, userLongitude, ptr->latitude, ptr->longitude))) {  // HTTPS
        int http_distCode = https_dist.GET();
  
        if (http_distCode > 0) {
          
          // file found at server
          if (http_distCode == HTTP_CODE_OK || http_distCode == HTTP_CODE_MOVED_PERMANENTLY) {
            http_distResult = https_dist.getString().substring(592);
            Serial.printf("\t--> %s : OK\n", ptr->id);
          }
        } else {
          requestFailed = true;
          Serial.printf("[HTTPS] GET... failed, error: %s, failure for : %s\n", https_dist.errorToString(http_distCode).c_str(), ptr->id);
        }
        
        https_dist.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }

    delay(2000);

    ptr++;

    if(!requestFailed)
      extractDataFromJson(http_distResult.c_str(), &(ptr-1)->parkingDist);
  }
}
