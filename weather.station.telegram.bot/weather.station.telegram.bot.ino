// weather.station.telegram.bot.ino
#include <Wire.h>
#include "Adafruit_BME280.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define I2C_SDA 21
#define I2C_SCL 22
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_ADD 0x76
#define TelegramApiToken "<telegram-api-token>"
#define TelegramChatID "<chat-id>"

// Initialize Wifi connection to the router
char ssid[] = "<network-name>";
char password[] = "<network-key>";
WiFiClientSecure client;

// Initialize Telegram Bot
UniversalTelegramBot bot(TelegramApiToken, client);

// Initialize BMP280 sensor
Adafruit_BME280 bme(I2C_SDA, I2C_SCL);

void getValues(void);

void setup() {
  Serial.begin(115200);
  Serial.println("Program Start");

  bool status;

  status = bme.begin(BME280_ADD);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void loop() {
  delay(1000);
  getValues();
  delay(1000);
}

void getValues() {

  String info = "";
  info.concat("Temperature: ");
  info.concat(bme.readTemperature());
  info.concat(" / ");
  info.concat("Approx. Altitude: ");
  info.concat(bme.readAltitude(SEALEVELPRESSURE_HPA));
  info.concat(" / ");
  info.concat("Pressure: ");
  info.concat(bme.readPressure() / 100.0F);
  info.concat(" / ");
  info.concat("Humidity: ");
  info.concat(bme.readHumidity());

  bot.sendMessage(TelegramChatID, info, "");

  delay(1000*60*15);
  Serial.println(info);
}
