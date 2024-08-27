#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> 
#include <SPI.h>

const char* ssid = "";
const char* password = "";
const char* discord_token = "";
const char* guild_id = "";
const char* channel_id = "";
const char* user_id = "";

const int kickButton = 14;
volatile bool isButtonPressed = false;

void IRAM_ATTR handleButtonPress() {
  isButtonPressed = true;
}

#define TFT_CS      10
#define TFT_DC      2
#define TFT_RST     4

/*
display
sck 18
sda 23
*/

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

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
    http.begin(url);
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
  http.begin("https://discord.com/api/v10/guilds/" + String(guild_id) + "/members?limit=1000");
  http.addHeader("Authorization", "Bot " + String(discord_token));
  int httpCode = http.GET();

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(60, 10);
  tft.println(httpCode);
  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);

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
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(60, 10);
    tft.println("Failed to get members list");
  }

  http.end();

  // Обновляем данные каждые 10 секунд (или другой интервал)
  delay(10000);
}