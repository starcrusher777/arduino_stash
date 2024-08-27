#include <Arduino_GFX_Library.h>

// Настройка подключения дисплея
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4

// Создаем интерфейс SPI
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, SCK, MOSI, MISO);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, 0);

void drawClockFace();
void drawHand(int angle, int length, int width);

void setup() {
  gfx->begin();
  gfx->fillScreen(BLACK);
}

void loop() {
  drawClockFace();
  delay(1000);
}

void drawClockFace() {
  gfx->fillScreen(BLACK);
  gfx->drawCircle(gfx->width() / 2, gfx->height() / 2, gfx->width() / 2 - 5, WHITE);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(1);

  // Рисуем отметки времени
  for (int i = 0; i < 12; i++) {
    float angle = i * 30 * PI / 180;
    int x = cos(angle) * (gfx->width() / 2 - 10) + gfx->width() / 2;
    int y = sin(angle) * (gfx->height() / 2 - 10) + gfx->height() / 2;
    gfx->fillCircle(x, y, 2, WHITE);
  }

  // Рисуем стрелки часов
  int hour = 3;    // Замените текущим часом
  int minute = 15; // Замените текущей минутой
  int second = 30; // Замените текущей секундой

  // Часовая стрелка
  drawHand(hour * 30 + minute / 2, gfx->width() / 2 - 20, 4);

  // Минутная стрелка
  drawHand(minute * 6, gfx->width() / 2 - 10, 2);

  // Секундная стрелка
  drawHand(second * 6, gfx->width() / 2 - 5, 1);
}

void drawHand(int angle, int length, int width) {
  float radian = angle * PI / 180;
  int x = cos(radian) * length + gfx->width() / 2;
  int y = sin(radian) * length + gfx->height() / 2;
  gfx->drawLine(gfx->width() / 2, gfx->height() / 2, x, y, WHITE);
}