#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <MD_MAX72XX.h>
#include <MD_Parola.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// Pin definitions
#define DATA_PIN  D4
#define CS_PIN    D0
#define CLK_PIN   D5

// Define the number of devices we have in the chain and the hardware interface
#define NUM_MATRICES 4
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

MD_Parola parola = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_MATRICES);
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_MATRICES);
ESP8266WebServer server(80);

// Text message to scroll
String message = "Hello, World!";
int scrollSpeed = 55;

// LED pins and status
uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

uint8_t Rpin = D1;
uint8_t Gpin = D2;
uint8_t Bpin = D3;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(Rpin, OUTPUT);
  pinMode(Gpin, OUTPUT);
  pinMode(Bpin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  delay(500);

  parola.begin();
  parola.setIntensity(0);
  parola.displayText(message.c_str(), PA_CENTER, scrollSpeed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  server.on("/", handle_OnConnect);
  server.on("/color", handle_ColorChange);
  server.on("/rootMatrix", handleRootMatrix);
  server.on("/rootText", handleRootText);
  server.on("/updateMatrix", HTTP_POST, handleUpdateMatrix);
  server.on("/setText", handleSetText);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  if (parola.displayAnimate()) {
    parola.displayReset();
  }
  
  digitalWrite(LED1pin, LED1status);
  digitalWrite(LED2pin, LED2status);
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(LED1status, LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  server.send(200, "text/html", SendHTML(true, LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  server.send(200, "text/html", SendHTML(false, LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  server.send(200, "text/html", SendHTML(LED1status, true)); 
}

void handle_led2off() {
  LED2status = LOW;
  server.send(200, "text/html", SendHTML(LED1status, false)); 
}

void handle_ColorChange() {
  String color = server.arg("color");
  long number = strtol(color.substring(1).c_str(), NULL, 16);
  
  byte r = (number >> 16) & 0xFF;
  byte g = (number >> 8) & 0xFF;
  byte b = number & 0xFF;

  analogWrite(Rpin, r);
  analogWrite(Gpin, g);
  analogWrite(Bpin, b);
  
  server.send(200, "text/html", SendHTML(LED1status, LED2status));
}

void handleRootMatrix() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
      <title>LED Matrix Editor</title>
      <style>
        html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
        body {margin-top: 50px;}
        table {margin: 0 auto;}
        td {width: 20px; height: 20px; border: 1px solid #000;}
        .active {background-color: #000;}
        .button {display: block; width: 100px; background-color: #1abc9c; border: none; color: white; padding: 10px; text-decoration: none; font-size: 16px; margin: 20px auto; cursor: pointer; border-radius: 4px;}
      </style>
    </head>
    <body>
      <h1>LED Matrix Editor</h1>
      <table id="matrix">
        <!-- The table cells will be added here by JavaScript -->
      </table>
      <button class="button" onclick="sendMatrix()">Send to LED Matrix</button>

      <script>
        const rows = 8;
        const cols = 8 * 4;  // NUM_MATRICES
        const table = document.getElementById('matrix');

        // Create the grid
        for (let i = 0; i < rows; i++) {
          const tr = document.createElement('tr');
          for (let j = 0; j < cols; j++) {
            const td = document.createElement('td');
            td.addEventListener('click', () => td.classList.toggle('active'));
            tr.appendChild(td);
          }
          table.appendChild(tr);
        }

        function sendMatrix() {
          const matrix = [];
          const cells = table.getElementsByTagName('td');
          for (let i = 0; i < cells.length; i++) {
            matrix.push(cells[i].classList.contains('active') ? 1 : 0);
          }
          fetch('/updateMatrix', {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json',
            },
            body: JSON.stringify({matrix}),
          }).then(response => {
            if (response.ok) {
              alert('Matrix updated successfully!');
            } else {
              alert('Failed to update matrix.');
            }
          });
        }
      </script>
    </body>
    </html>
  )rawliteral");
}

void handleRootText() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
      <title>LED Matrix Text</title>
      <style>
        html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
        body {margin-top: 50px;}
        .button {display: block; width: 100px; background-color: #1abc9c; border: none; color: white; padding: 10px; text-decoration: none; font-size: 16px; margin: 20px auto; cursor: pointer; border-radius: 4px;}
      </style>
    </head>
    <body>
      <h1>LED Matrix Text</h1>
      <form action="/setText" method="get">
        <input type="text" name="text" value="" placeholder="Enter text">
        <input type="submit" value="Set Text">
      </form>
    </body>
    </html>
  )rawliteral");
}

void handleUpdateMatrix() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  DynamicJsonDocument doc(1024);
  String body = server.arg("plain");
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    server.send(400, "text/plain", "Bad Request");
    return;
  }

  JsonArray matrix = doc["matrix"].as<JsonArray>();

  mx.clear();
  for (int matIndex = 0; matIndex < NUM_MATRICES; matIndex++) {
    int matrixOffset = matIndex * 8;

    for (int col = 0; col < 8; col++) {
      byte colData = 0;
      for (int row = 0; row < 8; row++) {
        if (matrix[matrixOffset + col + row * NUM_MATRICES * 8]) {
          colData |= (1 << row);
        }
      }
      mx.setColumn(matIndex * 8 + col, colData);
    }
  }

  server.send(200, "text/plain", "Matrix updated successfully");
}

void handleSetText() {
  String newText = server.arg("text");
  if (newText.length() > 0) {
    message = newText;
    parola.displayText(message.c_str(), PA_CENTER, scrollSpeed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    server.send(200, "text/html", SendHTML(LED1status, LED2status));
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP8266 Web Server</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body {margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";

  if (led1stat) {
    ptr += "<p>LED 1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  } else {
    ptr += "<p>LED 1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  if (led2stat) {
    ptr += "<p>LED 2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  } else {
    ptr += "<p>LED 2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
  }

  ptr += "<p><a href=\"/rootMatrix\">Matrix Editor</a></p>";
  ptr += "<p><a href=\"/rootText\">Set Text</a></p>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}