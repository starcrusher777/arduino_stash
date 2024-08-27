#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library#include <Adafruit_GFX.h>    // Core graphics libr
#include <SD.h>
#include <FS.h>

// ST7789 TFT module connections
#define TFT_CS    -1  // define chip select pin
#define TFT_DC     2  // define data/command pin
#define TFT_RST    4  // define reset pin, or set to -1 and connect to Arduino RESET pin
#define CD_CS      5
#define BUFFPIXEL 40

void playVideo();
boolean readBMP(File &f, Adafruit_ST7789 &tft);
uint16_t read16(File &f);
uint32_t read32(File &f);
/*
#define TFT_RST    8  
#define TFT_DC     9  
#define TFT_SCK    13 
#define TFT_MOSI   11 
#define TFT_CS     -1
*/

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);


float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
 
  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE3);    // Init ST7789 display 240x240 pixel
 
  // if the screen is flipped, remove this command
  tft.setRotation(2); // Поворот экрана (значение от 0 до 3)
  tft.fillScreen(ST77XX_BLACK); // Заливка экрана черным цветом

  
  // Initialize SD card
  if (!SD.begin(CD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  File bmpFile = SD.open("/robot.bmp");
  if (bmpFile) {
    Serial.println("robot.bmp opened successfully.");
    if (!readBMP(bmpFile, tft)) {
      Serial.println("Failed to load image.");
    }
    bmpFile.close();
  }

  playVideo(); // Start playing video
}


void loop() {
 
}

void playVideo() {
  int frameNumber = 1; // Start from frame0001.bmp
  while (frameNumber <= 6) { // Воспроизведение до frame0031.bmp
    char filename[20];
    sprintf(filename, "/frame%04d.bmp", frameNumber);
    Serial.print("Attempting to display: ");
    Serial.println(filename);

    if (!SD.exists(filename)) {
      // Если файл не найден, завершить цикл
      Serial.println("File not found, stopping...");
      break;
    }

    File bmpFile = SD.open(filename);
    if (!bmpFile) {
      Serial.println("Failed to open file for reading");
      break;
    }
    Serial.println("File opened successfully."); // Добавлено для отладки
    
    if (!readBMP(bmpFile, tft)) {
    Serial.println("Failed to load image.");
}

    bmpFile.close();
    delay(33); // Задержка между кадрами (30 fps)
    frameNumber++;
  }
}



boolean readBMP(File &f, Adafruit_ST7789 &tft) {
  uint32_t bmpImageoffset;
  uint32_t rowSize;
  uint16_t w, h, row, col;
  uint8_t r, g, b;
  uint8_t buff[3 * BUFFPIXEL];
  boolean goodBmp = false;
  boolean flip = true;

  if (read16(f) == 0x4D42) { // BMP signature
    Serial.println("BMP signature found.");
    uint32_t fileSize = read32(f); // File size
    Serial.print("File size: ");
    Serial.println(fileSize);
    bmpImageoffset = read32(f); // Offset to image data
    Serial.print("Image offset: ");
    Serial.println(bmpImageoffset);
    uint32_t reserved = read32(f); // Reserved
    Serial.print("Reserved: ");
    Serial.println(reserved);
    w = read32(f); // Image width
    h = read32(f); // Image height
    Serial.print("Image size: ");
    Serial.print(w);
    Serial.print("x");
    Serial.println(h);
    uint16_t planes = read16(f); // Planes
    Serial.print("Planes: ");
    Serial.println(planes);
    uint16_t bitDepth = read16(f); // Bit depth
    Serial.print("Bit depth: ");
    Serial.println(bitDepth);
    uint32_t compression = read32(f); // Compression
    Serial.print("Compression: ");
    Serial.println(compression);

    if (planes == 1 && bitDepth == 24 && compression == 0) { // 24-bit BMP
      Serial.println("24-bit BMP format confirmed.");
      goodBmp = true;

      rowSize = (w * 3 + 3) & ~3;

      for (row = 0; row < h; row++) {
        Serial.print("Processing row: ");
        Serial.println(row); // Debugging
        uint32_t pos = bmpImageoffset + (flip ? (h - 1 - row) * rowSize : row * rowSize);
        f.seek(pos);
        uint8_t buffidx = sizeof(buff);
        for (col = 0; col < w; col++) {
          if (buffidx >= sizeof(buff)) {
            f.read(buff, sizeof(buff));
            buffidx = 0;
          }
          b = buff[buffidx++];
          g = buff[buffidx++];
          r = buff[buffidx++];
          tft.drawPixel(col, row, tft.color565(r, g, b));
        }
      }
    } else {
      Serial.println("Not a 24-bit BMP file or unsupported BMP format.");
    }
  } else {
    Serial.println("Not a BMP file.");
  }
  return goodBmp;
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}