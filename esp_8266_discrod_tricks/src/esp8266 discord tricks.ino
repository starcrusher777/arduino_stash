#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h> // Добавлено
#include <SPI.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

const char* ssid = "";
const char* password = "";
const char* discord_token = "";
const char* guild_id = "";
const char* channel_id = "";
const char* user_id = "";

const int kickButton = D8;
volatile bool isButtonPressed = false;

void IRAM_ATTR handleButtonPress() {
  isButtonPressed = true;
}

#define TFT_CS      D1
#define TFT_DC      D2
#define TFT_RST     D3

/*
display
sck D5
sda D7
*/

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

WiFiClient wifiClient;


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  tft.init(240, 240, SPI_MODE3);

  pinMode(kickButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(kickButton), handleButtonPress, FALLING);
}

void loop() {

  if (isButtonPressed) {
    isButtonPressed = false;

    HTTPClient http;
    String url = "https://discord.com/api/v10/guilds/" + String(guild_id) + "/members/" + String(user_id);
    http.begin(wifiClient, url);
    http.addHeader("Authorization", "Bot " + String(discord_token));
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"channel_id\": null}";
    int httpCode = http.PATCH(payload);
    if (httpCode > 0) {
      Serial.println("HTTP PATH response code: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("User disconnected successfully.");
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(60, 10);
        tft.println("User disconnected successfully.");
      } else {
        Serial.println("Failed to disconnect user. Error: " + http.getString());
        tft.setRotation(2);
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(60, 30);
        tft.println("Failed to disconnect user. Error: " + http.getString());
      }
    } else {
      Serial.println("HTTP PATH request failed.");
    }

    http.end();

  }
  HTTPClient http;
  http.begin(wifiClient, "https://discord.com/api/v10/guilds/" + String(guild_id) + "/members?limit=1000");
  http.addHeader("Authorization", "Bot " + String(discord_token));
  int httpCode = http.GET();

  tft.setRotation(2);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(60, 10);
  tft.println(httpCode);
  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, payload);

    // Очищаем экран перед выводом новых данных
    tft.setRotation(2);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0, 0);
    tft.setTextSize(1);

    for (JsonVariant value : doc.as<JsonArray>()) {
      JsonObject member = value.as<JsonObject>();
      const char* username = member["user"]["username"];
      tft.println(username);
    }
  } else {
    Serial.println("Failed to get members list");
    tft.setRotation(2);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(60, 10);
    tft.println("Failed to get members list");
  }

  http.end();

  // Обновляем данные каждые 10 секунд (или другой интервал)
  delay(10000);
}