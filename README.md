# arduino_stash

A collection of projects for Arduino and compatible boards (ESP8266, ESP32, etc.). Projects include displays, web servers, wireless communication, sensors, and other experiments.

## Projects

| Project | Description |
|---------|-------------|
| **7PosDiode** | Seven-segment display |
| **AnimPlay** | Animation playback |
| **CatHittingTableIdk** | Cat-themed project (table hitting) |
| **eSPI** | TFT display (TFT_eSPI) on ESP32 |
| **esp_8266_discrod_tricks** | ESP8266 integration with Discord |
| **gifPlayer** | GIF playback on display |
| **hum_temp_sensors** | Humidity and temperature sensors |
| **kickButton_discord_members_list** | Button and Discord members list |
| **NewTFTDisplay** | TFT display test/demo |
| **OLED+menu** | OLED display with menu |
| **roundDisplay** | Round display |
| **serialport** | Serial port (incl. CatHittingTableOnWinKeyboard, SerialPortESP8266) |
| **test** | Test project |
| **tft_test_esp_32** | TFT test on ESP32 |
| **UnoOLEDtest** | OLED test on Arduino Uno |
| **UnoSimpleGame** | Simple game on Arduino Uno |
| **WebServer_matrix_draw_table** | Web server with matrix drawing |
| **WebServer_moving_text** | Web server with scrolling text |
| **WebSocket** | WebSocket usage |
| **wireless** | Wireless projects (Wireless Cat, WirelessCatControl) |

---

## Using with Arduino IDE

### Setup

1. Install [Arduino IDE](https://www.arduino.cc/en/software) (2.x recommended).
2. For **ESP8266** or **ESP32** boards, add board support via **File → Preferences → Additional boards manager URLs**:
   - ESP8266: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - ESP32: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Install the boards via **Tools → Board → Boards Manager** (search for “esp8266” or “esp32” and install).
4. Install libraries via **Tools → Manage Libraries** if needed.

### Opening and uploading a project

1. Open the folder of the project you want (e.g. `WebServer_matrix_draw_table`).
2. In Arduino IDE: **File → Open** and select the `.ino` file (often in `src/`, e.g. `src/HelloServer.ino`).
3. Select the board: **Tools → Board** (Arduino Uno, NodeMCU, ESP32 Dev Module, etc.).
4. Select the port: **Tools → Port**.
5. Click **Upload** (right arrow) or **Sketch → Upload**.

For projects with a PlatformIO-style layout (`src/`, `lib/`, `platformio.ini`), open the main `.ino` or `.cpp` from `src/` in Arduino IDE and add any required libraries manually.

---

## Using with VSCode and PlatformIO

### Installation

1. Install [Visual Studio Code](https://code.visualstudio.com/).
2. Open the Extensions view (**Ctrl+Shift+X**), search for **PlatformIO IDE** and install it (PlatformIO will download the required tools on first run).

### Opening and building a project

1. Use **File → Open Folder** and select the **project folder** (e.g. `arduino_stash/eSPI` or `arduino_stash/WebServer_matrix_draw_table`), not the repo root.
2. PlatformIO will detect `platformio.ini` and fetch the platform and libraries.
3. In the PlatformIO bottom bar:
   - **Build** (checkmark) — build the project.
   - **Upload** (right arrow) — upload firmware to the board.
   - **Clean** — clean the build.
4. Port and board can be chosen in **PlatformIO → Project Tasks** or via `platformio.ini` (`upload_port` and `[env:...]` sections).

### Multiple environments

If `platformio.ini` defines several `[env:...]` sections (e.g. `[env:uno]` and `[env:esp32dev]`):

- Pick the desired environment in **Project Tasks**.
- Or in the terminal:
  ```bash
  pio run -e esp32dev
  pio run -e esp32dev --target upload
  ```

### Useful terminal commands (from the project folder)

```bash
# Build
pio run

# Upload to board
pio run --target upload

# Build and upload for a specific environment (e.g. esp32dev)
pio run -e esp32dev --target upload

# Clean
pio run --target clean

# Open serial monitor
pio device monitor
```

---

## Typical project structure

- **platformio.ini** — PlatformIO config (platform, board, libraries).
- **src/** — Source code (`.ino`, `.cpp`).
- **lib/** — Local libraries (if any).
- **include/** — Headers (if any).

See the `README.md` inside each project folder for project-specific details (when present).
