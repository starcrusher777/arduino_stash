#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library#include <Adafruit_GFX.h>    // Core graphics library
#include "FontsRus/TimesNRCyr10.h"



// ST7789 TFT module connections
#define TFT_CS    10  // define chip select pin
#define TFT_DC     2  // define data/command pin
#define TFT_RST    4  // define reset pin, or set to -1 and connect to Arduino RESET pin

/*
display
sck 18
sda 23

sd card
cs 5
sck 18
mosi 23
miso 19
*/

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
 
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE3);    // Init ST7789 display 240x240 pixel SPI_MODE3 = esp 32 / SPI_MODE2 = Uno
 
  // if the screen is flipped, remove this command
  tft.setRotation(2); // Поворот экрана (значение от 0 до 3)
  tft.fillScreen(ST77XX_WHITE); // Заливка экрана черным цветом

  // Отображение тестового текста
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 10);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Довели! Довольны?");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 35);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 55);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 75);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 95);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 115);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 135);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(10, 155);
  tft.setFont(&TimesNRCyr10pt8b);
  tft.println("Да, довольны");


  tft.setFont();
}
 
void loop() {
 
}