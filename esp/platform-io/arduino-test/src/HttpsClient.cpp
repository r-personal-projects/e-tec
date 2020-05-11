#include "HttpsClient.h"
#include <Arduino.h>
#include "SSD1306Wire.h"
#include "constants.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WiFi.h>

SSD1306Wire d(0x3c, MY_DISPLAY_SCA, MY_DISPLAY_SCL);

WiFiClientSecure httpsClient;

HttpsClient::~HttpsClient()
{

}

HttpsClient::HttpsClient() {
    retry = 0;
}

void HttpsClient::makeRequest(String host, String path, String additionalHeader, String body, String method) {
    retry = 0;
    httpsClient.setTimeout(15000);
    delay(200);

    Serial.print("Connecting to " + host);
    while ((!httpsClient.connect(host.c_str(), 443)) && (retry++ < 15))
    {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    if (retry > 15)
    {
        Serial.println("Connection to " + host + " failed.");
    }
    else
    {
        Serial.println("Connection to " + host + " was successfull.");

        httpsClient.println(method + " " + path + " HTTP/1.1");
        httpsClient.println("Host: " + host);
        if (additionalHeader == "")
        {
            Serial.println("additional Headers are empty");
        } else {
            Serial.println("Using additional headers: " + additionalHeader);
            httpsClient.println(additionalHeader);
        }
        
        httpsClient.println("Connection: close");
        
        if(body == "") {
            Serial.println("body is empty");
        } else {
            Serial.println("Using body: " + body);
            httpsClient.print("Content-Length: ");
            httpsClient.println(body.length());
            httpsClient.println();
            httpsClient.println(body);
        }
        
        while (httpsClient.connected())
        {
            String line = httpsClient.readStringUntil('\n');
            if (line == "\r")
            {
                Serial.println("headers received");
                break;
            }
        }
        // if there are incoming bytes available
        // from the server, read them and print them:
        while (httpsClient.available())
        {
            String s = httpsClient.readString();
            DynamicJsonDocument doc(20000);
            DeserializationError error = deserializeJson(doc, s);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                
            } else
            {
                Serial.println(F("Response:"));
                Serial.println(doc["connections"].as<char*>());
            }
            

            
        }

        Serial.println("--------------");
        Serial.println("Request was: ");
        Serial.println(method + " " + path + " HTTP/1.1");
        Serial.println("Host: " + host);
        Serial.println(additionalHeader);
        Serial.println("Connection: close");
        Serial.print("Content-Length: ");
        Serial.println(body.length());
        Serial.println();
        Serial.println(body);
        Serial.println("--------------");

        httpsClient.stop();
    }
}

void HttpsClient::doGet(String host, String path) {
    makeRequest(host, path, "", "", "GET");
}

void HttpsClient::doGet(String host, String path, String additionalHeader) {
    makeRequest(host, path, additionalHeader, "", "GET");
}

void HttpsClient::doPost(String host, String path, String body, String additionalHeader) {
    makeRequest(host, path, additionalHeader, body, "POST");
}

IPAddress initWifiClient(String ssid, String pass) {
    Serial.println("Connecting to: " + ssid + " || " + pass);
    WiFi.begin(ssid.c_str(), pass.c_str());
    int retry = 0;
    WiFi.mode(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
        if (retry++ == 20)
        {
            ESP.restart();
        }
    }

    Serial.println();

    return WiFi.localIP();
}