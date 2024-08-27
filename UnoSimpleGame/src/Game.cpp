#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define JOYSTICK_VRX A0
#define JOYSTICK_VRY A1
#define JOYSTICK_SW  7
#define BATTERY_PIN A0

enum GameState { MENU, GAMES_MENU, GAME, GAME_OVER };
GameState currentState = MENU;

// Параметры игрока
int playerX = 10;
int playerY = 50;
int playerWidth = 10;
int playerHeight = 10;
int playerVelocityY = 0;
int gravity = 1;
int jumpStrength = -10;
bool isJumping = false;

// Параметры препятствий
int obstacleX = SCREEN_WIDTH;
int obstacleY = 50;
int obstacleWidth = 10;
int minObstacleHeight = 5;
int maxObstacleHeight = 30; // Начальная максимальная высота препятствия
int obstacleHeight = random(minObstacleHeight, maxObstacleHeight);
int obstacleSpeed = 3;

int score = 0;

void drawMenu();
void selectMenuItem();
void drawGamesMenu();
void selectGameMenu();
void drawStatusBar();
void drawBatteryIcon(int level);
int getBatteryLevel();
void gameOver();
void drawGame();
void updateGame();
void checkJoystick();

const char* menuItems[] = {
  "Games",
  "Battery Status",
  "Exit"
};
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
int currentIndex = 0;

const char* gamesMenuItems[] = {
  "Jump",
  "Race"
};
const int gamesMenuLength = sizeof(gamesMenuItems) / sizeof(gamesMenuItems[0]);
int currentGameIndex = 0;

void setup() {
  pinMode(JOYSTICK_SW, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  drawMenu();
}

void loop() {
  checkJoystick();
  switch (currentState) {
    case MENU:
      break;

    case GAMES_MENU:
      break;

    case GAME:
      if (analogRead(JOYSTICK_VRY) < 400 && !isJumping) {
        playerVelocityY = jumpStrength;
        isJumping = true;
      }
      updateGame();
      drawGame();
      delay(30);
      break;

    case GAME_OVER:
      if (digitalRead(JOYSTICK_SW) == LOW) {
        currentState = MENU;
        drawMenu();
        delay(200);
      }
      break;
  }
}

void checkJoystick() {
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 200;

  int vrx = analogRead(JOYSTICK_VRX);
  int vry = analogRead(JOYSTICK_VRY);
  int sw = digitalRead(JOYSTICK_SW);

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentState == MENU || currentState == GAMES_MENU) {
      if (vrx > 600) {
        currentIndex++;
        if (currentState == MENU) {
          if (currentIndex >= menuLength) {
            currentIndex = 0;
          }
          drawMenu();
        } else if (currentState == GAMES_MENU) {
          if (currentIndex >= gamesMenuLength) {
            currentIndex = 0;
          }
          drawGamesMenu();
        }
        lastDebounceTime = millis();
      } else if (vrx < 400) {
        currentIndex--;
        if (currentState == MENU) {
          if (currentIndex < 0) {
            currentIndex = menuLength - 1;
          }
          drawMenu();
        } else if (currentState == GAMES_MENU) {
          if (currentIndex < 0) {
            currentIndex = gamesMenuLength - 1;
          }
          drawGamesMenu();
        }
        lastDebounceTime = millis();
      }

      if (sw == LOW) {
        if (currentState == MENU) {
          selectMenuItem();
        } else if (currentState == GAMES_MENU) {
          selectGameMenu();
        }
        lastDebounceTime = millis();
      }
    }
  }
}

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

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

void drawGamesMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  for (int i = 0; i < gamesMenuLength; i++) {
    if (i == currentGameIndex) {
      display.print("> ");
    } else {
      display.print(" ");
    }
    display.println(gamesMenuItems[i]);
  }
  display.display();
}

void selectMenuItem() {
  if (strcmp(menuItems[currentIndex], "Games") == 0) {
    currentState = GAMES_MENU;
    display.clearDisplay();
    drawGamesMenu();
    display.display();
  } else if (strcmp(menuItems[currentIndex], "Battery Status") == 0) {
    display.clearDisplay();
    drawStatusBar();
    display.display();
    delay(2000);
    drawMenu();
  } else if (strcmp(menuItems[currentIndex], "Exit") == 0) {
    // TODO
  }
}

void selectGameMenu() {
  if (strcmp(gamesMenuItems[currentGameIndex], "Jump") == 0) {
    currentState = GAME;
    playerX = 10;
    playerY = 50;
    playerVelocityY = 0;
    obstacleX = SCREEN_WIDTH;
    isJumping = false;
    score = 0; // Сбросить очки при начале новой игры
  } else if (strcmp(gamesMenuItems[currentGameIndex], "Race") == 0) {
    //todo
  }
}

void drawStatusBar() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  int batteryLevel = getBatteryLevel();
  drawBatteryIcon(batteryLevel);
}

void drawBatteryIcon(int level) {
  int batteryWidth = 24;
  int batteryHeight = 10;
  int batteryX = SCREEN_WIDTH - batteryWidth - 2;
  int batteryY = 0;

  // Battery
  display.drawRect(batteryX, batteryY, batteryWidth, batteryHeight, SSD1306_WHITE);
  display.drawRect(batteryX + batteryWidth, batteryY + 2, 2, batteryHeight - 4, SSD1306_WHITE);

  // Draw battery
  int fillWidth = map(level, 0, 100, 0, batteryWidth - 2);
  display.fillRect(batteryX + 1, batteryY + 1, fillWidth, batteryHeight - 2, SSD1306_WHITE);

  display.setCursor(batteryX - 30, batteryY + 2);
  display.print(level);
  display.print("%");
}

int getBatteryLevel() {
  int sensorValue = analogRead(BATTERY_PIN);
  int batteryLevel = map(sensorValue, 0, 1023, 0, 100);
  return batteryLevel;
}

void gameOver() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.print("Game Over!");
  display.display();
  currentState = GAME_OVER;
}

void drawGame() {
  display.clearDisplay();
  display.fillRect(playerX, playerY, playerWidth, playerHeight, SSD1306_WHITE);
  display.fillRect(obstacleX, obstacleY, obstacleWidth, obstacleHeight, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  display.display();
}

void updateGame() {
  playerY += playerVelocityY;
  playerVelocityY += gravity;

  // Нижняя граница
  if (playerY >= SCREEN_HEIGHT - playerHeight) {
    playerY = SCREEN_HEIGHT - playerHeight;
    playerVelocityY = 0;
    isJumping = false;
  }

  // Верхняя граница
  if (playerY < 0) {
    playerY = 0;
    playerVelocityY = 0;
  }

  // Обновление препятствия
  obstacleX -= obstacleSpeed;
  if (obstacleX < -obstacleWidth) {
    obstacleX = SCREEN_WIDTH;
    obstacleHeight = random(minObstacleHeight, maxObstacleHeight);
    score++;
  }

  if(score > 0 && score <= 10) {
    obstacleSpeed = 3;
    maxObstacleHeight = 30;
  }
  else if (score > 10 && score <= 20) {
    obstacleSpeed = 4;
    maxObstacleHeight = 40;
  }
  else if (score > 20 && score <= 30) {
    obstacleSpeed = 5;
    maxObstacleHeight = 50;
  }
  else if (score > 30 && score <= 40) {
    obstacleSpeed = 6;
    maxObstacleHeight = 60;
  }
  else if (score > 40) {
    obstacleSpeed = 7;
    maxObstacleHeight = 70;
  }

  // Проверка на столкновение
  if (playerX < obstacleX + obstacleWidth && playerX + playerWidth > obstacleX &&
      playerY < obstacleY + obstacleHeight && playerY + playerHeight > obstacleY) {
    gameOver();
  }
}
