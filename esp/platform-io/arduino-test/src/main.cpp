#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include "fonts.h"
#include <string.h>
#include "HttpsClient.h"
#include "constants.h"


SSD1306Wire display(0x3c, MY_DISPLAY_SCA, MY_DISPLAY_SCL); // ADDRESS, SDA, SCL

HttpsClient https;

IPAddress ip;

String ssid = "WalLAN Basis";
String password = "87763687354308895036";

void setupWifi()
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Roboto_12);
  String s = "Connecting to wifi...\n" + ssid;

  display.drawString(0, 0, s);
  display.display();

  ip = initWifiClient(ssid, password);
  display.drawString(0, 34, "IP: " + ip.toString());
  display.display();
  delay(3000);
}

void setupDisplay()
{
  display.init();

  display.flipScreenVertically();
  display.setFont(Roboto_10);
}

int retry = 0;

/*void doGet(String host, String path, String additionalHeader)
{
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

    httpsClient.println("GET " + path + " HTTP/1.0");
    httpsClient.println("Host: " + host);
    httpsClient.println("Connection: close");
    if (additionalHeader == "")
    {
      Serial.println("additional Headers are empty");
    }
    
    httpsClient.println();

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
      char c = httpsClient.read();
      Serial.write(c);
    }

    httpsClient.stop();
  }
}

void doGet(String host, String path) {
  doGet(host, path, "");
}

void testCall()
{
  httpsClient.setTimeout(15000);
  delay(1000);
  int retry = 0;
  while ((!httpsClient.connect("www.howsmyssl.com", 443)) && (retry < 15))
  {
    delay(100);
    Serial.print(".");
    retry++;
  }
  if (retry == 15)
  {
    Serial.println("Connection failed");
  }
  else
  {
    Serial.println("Connected to Server");
  }

  httpsClient.print(String("GET ") + "/test.php?get=test" +
                    "HTTP/1.1\r\n" +
                    "Host: " + "www.aeq-web.com" +
                    "\r\n" + "Connection: close\r\n\r\n");
  while (httpsClient.connected())
  {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }
  String datarx;
  while (httpsClient.available())
  {
    datarx += httpsClient.readStringUntil('\n');
  }
  Serial.println(datarx);
  datarx = "";
}

void doPost(String a, String b, String c) {

}*/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  setupDisplay();
  setupWifi();

  String host = "www.l.de";
  String path = "/verkehrsbetriebe/fahrplan";
  String body = "results%5B5%5D%5B2%5D%5Bfunction%5D=ws_info_stop&results%5B5%5D%5B2%5D%5Bdata%5D=%5B%7B%22name%22%3A%22results%5B5%5D%5B2%5D%5Bstop%5D%22%2C%22value%22%3A%22Leipzig%2C+S%C3%BCdstr.%22%7D%2C%7B%22name%22%3A%22results%5B5%5D%5B2%5D%5Bdate%5D%22%2C%22value%22%3A%2212.01.2020%22%7D%2C%7B%22name%22%3A%22results%5B5%5D%5B2%5D%5Btime%5D%22%2C%22value%22%3A%22%22%7D%2C%7B%22name%22%3A%22results%5B5%5D%5B2%5D%5Bmode%5D%22%2C%22value%22%3A%22stop%22%7D%5D";
  String headers = "Content-Type: application/x-www-form-urlencoded; charset=UTF-8";
  https.doPost(host, path, body, headers);
}

void drawFontFaceDemo()
{
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Roboto_10);
  display.drawString(0, 0, "Nächste Straßenbahn");
  display.setFont(Roboto_10);
  display.drawString(0, 10, "  nach Böhlitz Ehrenberg");
  display.setFont(Roboto_16);
  display.drawString(0, 20, "    in 3 Minuten");

  display.setFont(Roboto_10);
  display.drawString(0, 36, "  nach Sommerfeld");
  display.setFont(Roboto_Bold_16);
  display.drawString(0, 46, "    in 5 Minuten");
}

void loop()
{
  // put your main code here, to run repeatedly:
  display.clear();
  drawFontFaceDemo();
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));

  display.display();
  delay(100);
}