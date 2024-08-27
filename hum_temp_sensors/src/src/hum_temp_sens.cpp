#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <DS1302.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define RTC_RST 10
#define RTC_DAT 11
#define RTC_CLK 13

Ds1302 rtc(RTC_RST, RTC_CLK, RTC_DAT);
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); // Адрес 0x27 для I2C

const char* dayOfWeek(int dayIndex) {
  const char* daysOfWeek[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
  return daysOfWeek[dayIndex - 1];
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  dht.begin();
  rtc.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000);

  if (rtc.isHalted()) {
    Serial.println("RTC остановлен. Установка времени...");
    Ds1302::DateTime dt = {
      .year = 24,
      .month = Ds1302::MONTH_JUL,
      .day = 11,
      .hour = 17,
      .minute = 37,
      .second = 0,
      .dow = Ds1302::DOW_THU
    };
    rtc.setDateTime(&dt);
  }

  lcd.clear();
}

void loop() { 
  Ds1302::DateTime t;
  rtc.getDateTime(&t);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Не удалось считать данные с датчика DHT11");
    return;
  }

  char dateTimeStr[17];
  snprintf(dateTimeStr, sizeof(dateTimeStr), "%02d-%02d-%04d %02d:%02d", t.day, t.month, t.year + 2000, t.hour, t.minute);

  const char* dow = dayOfWeek(t.dow);

  char tempHumStr[10];
  snprintf(tempHumStr, sizeof(tempHumStr), "%02dC %02d%%", (int)temperature, (int)humidity);

  char statusStr[17];
  snprintf(statusStr, sizeof(statusStr), "%s", dow);

  int spaceCount = 16 - strlen(statusStr) - strlen(tempHumStr);
  for (int i = 0; i < spaceCount; i++) {
    strcat(statusStr, " ");
  }
  strcat(statusStr, tempHumStr);

  lcd.setCursor(0, 0);
  lcd.print(dateTimeStr);
  lcd.setCursor(0, 1);
  lcd.print(statusStr);

  delay(1000);
}