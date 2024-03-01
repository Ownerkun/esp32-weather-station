#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "" //Blyn_template_id
#define BLYNK_TEMPLATE_NAME "" //Blynk_template_name
#define BLYNK_DEVICE_NAME "" //Blynk_device_name

#include <ESP.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"
#include "DHT.h"
#include <BMP085.h>
#include <Wire.h>

float temperature;
float humidity;
float uv;
float bmp_temperature;
float pressure;
float atm;
float altitude;

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

BMP085 myBarometer;

BlynkTimer timer;

char auth[] = ""; //Blynk_auth_token
char ssid[] = ""; //Wi-fi SSID
char pass[] = ""; //Wi-fi Password

void sendSensor()
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  uv = analogRead(34);
  bmp_temperature = myBarometer.bmp085GetTemperature(
    myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
  pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
  altitude = myBarometer.calcAltitude(101900);
  atm = pressure / 101325;

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  if (isnan(pressure))
  {
    Serial.println("Failed to read from BMP180 sensor!");
    return;
  }

}

void setup()
{
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  myBarometer.init();
  delay(1000);
  Blynk.begin(auth, ssid, pass); 
  delay(1000);
  timer.setInterval(10000L, sendSensor); // Send sensor data every 2 seconds
}

void loop()
{
  Blynk.run();
  timer.run();
}