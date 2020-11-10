/*
 * WiFiWebClient
 *
 * This sketch connects to a web server (http://www.google.com)
 * using the ISM43362-M3G-L44 WiFi module.
 *
 * This example is written for a network using WPA encryption.
 * For WEP or WPA, change the Wifi.begin() call accordingly.
 */
#include <Arduino.h>
#include <SPI.h>
#include <WiFiST.h>
#include "ThingSpeak.h"

/*
  The following configuration is dedicated to the DISCO L475VG IoT board.
  You should adapt it to your board.

Configure SPI3:
 * MOSI: PC12
 * MISO: PC11
 * SCLK: PC10

Configure WiFi:
 * SPI         : SPI3
 * Cs          : PE0
 * Data_Ready  : PE1
 * reset       : PE8
 * wakeup      : PB13
 */

SPIClass SPI_3(PC12, PC11, PC10);
WiFiClass WiFi(&SPI_3, PE0, PE1, PE8, PB13);

char ssid[] = "iPhone";   //  your network SSID (name)
char pass[] = "abcd1234"; // your network password
int keyIndex = 0;         // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = 1103978;
const char *myWriteAPIKey = "7A33Z8I8CVDB7GOW";

int number = 20;

void setup()
{
  Serial.begin(115200); // Initialize serial

  ThingSpeak.begin(client); //Initialize ThingSpeak
}

void loop()
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
  if (x == 200)
  {
    Serial.println("Channel update successful.");
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // change the value
  number++;
  Serial.println("Count up");
  if (number > 99)
  {
    number = 0;
  }

  delay(20000); // Wait 20 seconds to update the channel again
}