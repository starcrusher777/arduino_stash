#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Укажите размеры дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin # (или -1 если подключен к reset пину Arduino)
#define OLED_ADDRESS  0x3C  // I2C адрес OLED дисплея

// Пины для кнопок
#define UP_BUTTON_PIN    2
#define DOWN_BUTTON_PIN  3
#define SELECT_BUTTON_PIN 4

// Пин для измерения заряда батареи
#define BATTERY_PIN A0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Пункты меню
const char* menuItems[] = {
  "Temperature",
  "Humidity",
  "Time",
  "Exit"
};
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);

// Состояние меню
int currentIndex = 0;

// Иконки батареи
const uint8_t batteryIcons[][8] PROGMEM = {
  {0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},  // 0-20%
  {0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x10},  // 20-40%
  {0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x10, 0x10},  // 40-60%
  {0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x10, 0x10, 0x10},  // 60-80%
  {0x1F, 0x11, 0x11, 0x1F, 0x10, 0x10, 0x10, 0x10}   // 80-100%
};

// Объявления функций
void drawMenu();
void selectMenuItem();
void drawStatusBar();
int getBatteryLevel();

void setup() {
  // Инициализация последовательного порта для отладки
  Serial.begin(9600);

  // Инициализация кнопок
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);

  // Инициализация OLED дисплея
  Serial.println("Initializing display...");
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println("Display initialized!");
  
  display.display();
  delay(1000);
  display.clearDisplay();
  
  drawMenu();
}

void loop() {
  if (digitalRead(UP_BUTTON_PIN) == LOW) {
    currentIndex--;
    if (currentIndex < 0) {
      currentIndex = menuLength - 1;
    }
    drawMenu();
    delay(200);
  }

  if (digitalRead(DOWN_BUTTON_PIN) == LOW) {
    currentIndex++;
    if (currentIndex >= menuLength) {
      currentIndex = 0;
    }
    drawMenu();
    delay(200);
  }

  if (digitalRead(SELECT_BUTTON_PIN) == LOW) {
    selectMenuItem();
    delay(200);
  }
}

void drawMenu() {
  display.clearDisplay();
  drawStatusBar();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);  // Начинаем рисовать меню ниже статус-бара

  for (int i = 0; i < menuLength; i++) {
    if (i == currentIndex) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    display.println(menuItems[i]);
  }
  
  display.display();
}

void selectMenuItem() {
  display.clearDisplay();
  drawStatusBar();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);  // Начинаем рисовать ниже статус-бара
  
  // Пример для температуры, вы можете адаптировать этот участок кода для своего датчика
  if (currentIndex == 0) {
    float temperature = 25.0; // Здесь вставьте код для чтения температуры
    display.print("Temperature: ");
    display.print(temperature);
    display.println(" C");
  }

  if (currentIndex == 1) {
    int humidity = 46;
    display.print("Humidity: ");
    display.print(humidity);
    display.println(" %");
  }

  if (currentIndex == 2) {
    int hour = 0;
    int minute = 30;
    display.print("Time: ");
    display.print(hour);
    display.print(":");
    display.println(minute);
  }
  
  display.display();
  delay(2000);  // Показ выбранного элемента в течение 2 секунд

  drawMenu();
}

void drawStatusBar() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  // Отображение уровня заряда батареи
  int batteryLevel = getBatteryLevel();
  int iconIndex = batteryLevel / 20; // Выбор иконки в зависимости от уровня заряда
  display.drawBitmap(0, 0, batteryIcons[iconIndex], 8, 8, SSD1306_WHITE);

  // Добавить дополнительные элементы статус-бара здесь
}

int getBatteryLevel() {
  // Пример получения уровня заряда батареи, предполагая, что батарея подключена к A0
  int sensorValue = analogRead(BATTERY_PIN);
  // Преобразование значения сенсора в уровень заряда батареи в процентах
  int batteryLevel = map(sensorValue, 0, 1023, 0, 100);
  return batteryLevel;
}