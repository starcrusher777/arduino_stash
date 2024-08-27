#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <FS.h>
#include <SPIFFS.h>
#include <AnimatedGIF.h>

// Пины дисплея ST7789
#define TFT_CS      10
#define TFT_DC      2
#define TFT_RST     4

// Объект дисплея
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Объект GIF-аниматора
AnimatedGIF gif;

// Буфер для кадра
uint16_t frameBuffer[240 * 240];

// Колбэк для открытия файла
void *GIFOpenFile(const char *filename, int32_t *pSize)
{
    File file = SPIFFS.open(filename);
    if (file)
    {
        *pSize = file.size();
        return (void *)&file;
    }
    return NULL;
}

// Колбэк для закрытия файла
void GIFCloseFile(void *pHandle)
{
    File *file = static_cast<File *>(pHandle);
    file->close();
}

// Колбэк для чтения файла
int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    File *file = static_cast<File *>(pFile->fHandle);
    return file->read(pBuf, iLen);
}

// Колбэк для позиционирования в файле
int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{
    File *file = static_cast<File *>(pFile->fHandle);
    file->seek(iPosition);
    return iPosition;
}

// Функция для отрисовки кадра
void drawGifFrame(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, usTemp[320];
    int x, y, iWidth;

    iWidth = pDraw->iWidth;
    if (iWidth > 240)
        iWidth = 240;

    y = pDraw->iY + pDraw->y; // текущая строка

    s = pDraw->pPixels;
    d = usTemp;
    for (int i = 0; i < iWidth; i++)
    {
        *d++ = pDraw->pPalette[*s++];
    }
    tft.drawRGBBitmap(pDraw->iX, y, usTemp, iWidth, 1);
}

void setup()
{
    Serial.begin(9600);
    if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount file system");
    return;
  }

    // Инициализация дисплея
    tft.init(240, 240, SPI_MODE3);
    tft.setRotation(1);  // Поверните экран, если необходимо
    tft.fillScreen(ST77XX_BLACK);

    // Инициализация файловой системы
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // Инициализация GIF-аниматора
    gif.begin(LITTLE_ENDIAN_PIXELS);

    // Открытие GIF-файла
}


void loop()
{
    if (gif.playFrame(false, 0))
    {
        // Кадр успешно декодирован и отрисован
    }
    else
    {
        // Закончился GIF файл, переоткрываем его для зацикливания
        gif.close();
        if (!gif.open("/hoh.gif", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, drawGifFrame))
        {
            Serial.println("Failed to open GIF file");
        }
    }

    delay(10); // Задержка для контроля скорости воспроизведения
}