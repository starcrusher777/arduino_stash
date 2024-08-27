#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library#include <Adafruit_GFX.h>    // Core graphics library

// ST7789 TFT module connections
#define TFT_CS    10  // define chip select pin
#define TFT_DC     9  // define data/command pin
#define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin

/*
#define TFT_RST    8  // Пин сброса (или -1, если не подключен)
#define TFT_DC     9  // Пин данных/команд
#define TFT_SCK    13 // SPI Clock
#define TFT_MOSI   11 // SPI MOSI
#define TFT_CS     -1
*/

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
 
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 display 240x240 pixel
 
  // if the screen is flipped, remove this command
  tft.setRotation(2); // Поворот экрана (значение от 0 до 3)
  tft.fillScreen(ST77XX_BLACK); // Заливка экрана черным цветом

  // Отображение тестового текста
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.println("Hello, ST7789!");

  tft.drawRect(10, 10, 50, 50, ST77XX_RED);  // Красный прямоугольник
  tft.fillCircle(120, 120, 30, ST77XX_GREEN); // Зеленый круг
}
 
void loop() {
 
}