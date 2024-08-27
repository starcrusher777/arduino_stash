#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <WebSocketsClient.h>
#include <Arduino.h>

const char* ssid = "";
const char* password = "";
const char* discord_token = "";
const char* gateway_host = "gateway.discord.gg";
const uint16_t gateway_port = 443;

const int kickButton = 14;
volatile bool isButtonPressed = false;

void IRAM_ATTR handleButtonPress() {
  isButtonPressed = true;
}

#define TFT_CS      10
#define TFT_DC      2
#define TFT_RST     4

static bool isIdentified = false;

/*
display
sck 18
sda 23
*/

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
WebSocketsClient WebSocket;

void webSocketsEvents(WStype_t type, uint8_t * payload, size_t length) {

        tft.setRotation(2);
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextSize(1);
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(0, 0);
        tft.println(type);
        Serial.println("WSType_t type: ");
        Serial.print(type);

  if (type == WStype_CONNECTED) {
    if (type == WStype_TEXT) {
      DynamicJsonDocument doc(8192);
    deserializeJson(doc, payload);

    Serial.println("Received data from WebSocket:");
    Serial.println((char*)payload);

    if (doc["t"] == "READY") {
      Serial.println("Successfully identified with Discord Gateway.");
      isIdentified = true;
    }

    if (doc["t"] == "VOICE_STATE_UPDATE") {
      const char* user_id = doc["d"]["user_id"];
      const char* channel_id = doc["d"]["channel_id"];

      if (channel_id != nullptr) {
        Serial.printf("User %s joined channel %s\n", user_id, channel_id);
        tft.setRotation(2);
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextSize(1);
        tft.setTextColor(ST77XX_WHITE);
        tft.setCursor(0, 0);
        tft.println("User %s joined channel %s\n");
        tft.println(user_id);
        tft.println(channel_id);
      } else {
        Serial.printf("User %s left the voice channel\n", user_id);
        tft.println("User %s left the voice channel\n");
        tft.println(user_id);
      }
    }
    }
  } else if (type == WStype_DISCONNECTED) {
    Serial.println("WebSocket disconnected.");
    isIdentified = false;
  }
}

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  HTTPClient http;
  http.begin("https://discord.com/api/v10/gateway");
  http.addHeader("Authorization", "Bot" + String(discord_token));

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Response");
    Serial.println(payload);
  } else {
    Serial.println("failed to get url");
  }

  WebSocket.beginSSL(gateway_host, gateway_port, "/?v=10&encoding=json");
  WebSocket.setAuthorization("", discord_token);
  WebSocket.onEvent(webSocketsEvents);
  Serial.println("Listening");

  tft.init(240, 240, SPI_MODE3);
}

void loop()
{
  WebSocket.loop();
  static bool isIdentified = false;
    if (WebSocket.isConnected() && !isIdentified) {
        isIdentified = true;
        String identifiedPayload = "{\"op\":2,\"d\":{\"token\":\"" + String(discord_token) + "\",\"intents\":513,\"properties\":{\"$os\":\"linux\",\"$browser\":\"esp32\",\"$device\":\"esp32\"}}}";
        WebSocket.sendTXT(identifiedPayload);
    }
    delay(1000);
  }
