# 🚗 ESP32 Wi-Fi Controlled RC Car

A high-performance, responsive Wi-Fi controlled RC car powered by an **ESP32** microcontroller and an **L298N Dual H-Bridge Motor Driver**. The ESP32 hosts its own standalone Wi-Fi Access Point (SoftAP) and serves an interactive, mobile-touch-friendly web controller — **no internet connection, local Wi-Fi router, or mobile app installation required!**

---

## 📸 Overview & Features

- **Standalone Access Point (SoftAP)**: Broadcasts its own private Wi-Fi network (`ESP32-RC-Car`). Works anywhere, indoors or outdoors.
- **Embedded Web Controller**: Built-in HTTP web server serving a dark-themed touch interface directly to your mobile or desktop browser.
- **Low-Latency Real-Time Control**: Instant touch response using `ontouchstart` / `ontouchend` and mouse events (`onmousedown` / `onmouseup`), with immediate auto-stop upon release.
- **Zero App Required**: Fully cross-platform — works on iOS, Android, Windows, macOS, and Linux via any standard web browser.
- **Dual H-Bridge Motor Control**: Differential skid-steering drive logic (Forward, Backward, Left, Right, Stop) via L298N.

---

## 🛠️ Hardware Requirements (Bill of Materials)

| Component | Quantity | Purpose |
| :--- | :---: | :--- |
| **ESP32 Dev Module** (ESP-WROOM-32 / 30 or 38 pins) | 1 | Microcontroller & Wi-Fi Server |
| **L298N Dual H-Bridge Motor Driver** | 1 | Motor power and directional control |
| **DC Gear Motors (TT Motors) + Wheels** | 2 or 4 | Drive wheels (differential drive) |
| **Robot Chassis Kit** | 1 | Car frame & caster wheel (if 2WD) |
| **Power Supply (7.4V – 12V)** | 1 | 2x 18650 Li-ion batteries or battery pack |
| **Jumper Wires** | — | Female-to-Female & Male-to-Female |
| **SPST Power Switch** *(Optional)* | 1 | Master power toggle |

---

## 🔌 Hardware Wiring & Pinout

### 1. ESP32 to L298N Motor Driver

Connect your ESP32 to the L298N Motor Driver using the following pins:

| L298N Motor Driver Pin | ESP32 GPIO Pin | Function / Direction | Description |
| :---: | :---: | :--- | :--- |
| **IN1** | **GPIO 26** | Right Motor Forward | Drives right-side motor(s) forward |
| **IN2** | **GPIO 27** | Right Motor Backward | Drives right-side motor(s) backward |
| **IN3** | **GPIO 14** | Left Motor Forward | Drives left-side motor(s) forward |
| **IN4** | **GPIO 12** | Left Motor Backward | Drives left-side motor(s) backward |
| **GND** | **GND** | Common Ground | **CRITICAL:** Tie ESP32 GND and L298N GND together! |

> [!CAUTION]
> **Common Ground is Crucial**: Always connect the ESP32 `GND` pin to the L298N `GND` terminal. Without a common ground reference, logic control signals cannot be reliably detected, resulting in erratic or unresponsive motor behavior.

---

### 2. Power Supply & Motor Connections

```
                     +---------------------------------------+
                     |         7.4V - 12V Battery Pack       |
                     +-------------------+-------------------+
                                         | (+)            | (-)
                                         v                v
+------------------+             +---------------+   +----+--------------+
|      ESP32       |             |  L298N Driver |   |   Common Ground   |
|                  |             |               |   +----+--------------+
|  VIN / 5V (Opt) <+-------------+ 5V Output     |        |
|  GND ------------+-------------+ GND Terminal <+--------+
|  GPIO 26 --------+------------>+ IN1           |
|  GPIO 27 --------+------------>+ IN2           |        +--------------+
|  GPIO 14 --------+------------>+ IN3           +------->| Left Motor   |
|  GPIO 12 --------+------------>+ IN4           | OUT3/4 +--------------+
+------------------+             |               |
                                 |        OUT1/2 +------->+--------------+
                                 +---------------+        | Right Motor  |
                                                          +--------------+
```

1. **Battery Connections**:
   - Connect **Battery Positive (+)** to the L298N **12V** terminal.
   - Connect **Battery Negative (-)** to the L298N **GND** terminal.
2. **ESP32 Power**:
   - **Option A (Recommended for 7.4V - 12V batteries)**: Leave the **5V EN jumper** in place on the L298N. Run a jumper wire from the L298N **5V terminal** to the ESP32 **VIN** (or **5V**) pin.
   - **Option B**: Power the ESP32 independently via USB (e.g. power bank). In this case, ensure the **ESP32 GND is still connected to the L298N GND**.
3. **Speed Enable Jumpers (ENA / ENB)**:
   - Ensure the black jumper caps on **ENA** and **ENB** remain installed on the 5V pins on the L298N for maximum torque/speed.
4. **Motors**:
   - Connect Right Motor to `OUT1` and `OUT2`.
   - Connect Left Motor to `OUT3` and `OUT4`.

---

## 🕹️ Motor Control Logic (Truth Table)

The car uses differential skid steering:

| Direction | IN1 (GPIO 26) | IN2 (GPIO 27) | IN3 (GPIO 14) | IN4 (GPIO 12) | Web Server Endpoint |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Forward** | `HIGH` | `LOW` | `HIGH` | `LOW` | `GET /F` |
| **Backward** | `LOW` | `HIGH` | `LOW` | `HIGH` | `GET /B` |
| **Turn Left** | `HIGH` | `LOW` | `LOW` | `HIGH` | `GET /L` |
| **Turn Right** | `LOW` | `HIGH` | `HIGH` | `LOW` | `GET /R` |
| **Stop** | `LOW` | `LOW` | `LOW` | `LOW` | `GET /S` |

---

## 💻 Software & Firmware Setup

### Prerequisites
1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software) (version 2.x recommended).
2. Add ESP32 Board support:
   - Open **File > Preferences** in Arduino IDE.
   - In **Additional boards manager URLs**, add:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to **Tools > Board > Boards Manager**, search for `esp32`, and install **esp32 by Espressif Systems**.

### Flashing the Code
1. Clone or download this repository.
2. Open [esp_car.ino](esp_car.ino) in Arduino IDE.
3. Under **Tools > Board**, select **ESP32 Dev Module** (or your specific ESP32 variant).
4. Under **Tools > Port**, select the COM port corresponding to your plugged-in ESP32.
5. Click **Upload** (arrow icon).
   *(Note: If the upload hangs on `Connecting.......`, press and hold the `BOOT` button on the ESP32 until the flashing begins).*

---

## 🚀 How to Connect & Drive

1. **Power On**: Turn on the battery power switch to supply the ESP32 and L298N.
2. **Connect Wi-Fi**:
   - On your phone, tablet, or PC, open Wi-Fi Settings.
   - Connect to the network:
     - **SSID:** `ESP32-RC-Car`
     - **Password:** `password123`
3. **Open the Controller**:
   - Open any browser (Chrome, Safari, Firefox, Edge).
   - Go to: **`http://192.168.4.1`**
4. **Drive**:
   - Press and hold the **▲**, **▼**, **◀**, **▶** buttons to navigate.
   - Release the button to automatically stop, or tap **■** to halt immediately.

---

## ⚙️ Customization

- **Change Wi-Fi Name or Password**:
  In [esp_car.ino](esp_car.ino), update lines 6-7:
  ```cpp
  const char *ssid = "My-Custom-Car";
  const char *password = "supersecret"; // Must be at least 8 characters
  ```
- **Reverse Motor Direction**:
  If a motor spins backward when you press Forward, swap the two output wires on the L298N screw terminal (`OUT1`/`OUT2` or `OUT3`/`OUT4`), or swap the corresponding GPIO numbers in the code.

---

## 🔍 Troubleshooting

| Issue | Cause | Solution |
| :--- | :--- | :--- |
| **ESP32 reboots when motors start (Brownout)** | Motors draw peak surge current causing voltage drop | Use a separate, dedicated battery for motors, or add a 1000µF capacitor across the power rails. Ensure ESP32 GND and L298N GND remain tied together. |
| **Motors hum or vibrate but do not rotate** | Insufficient voltage or drained batteries | Ensure battery voltage is > 7.4V. Verify the black jumper caps on ENA & ENB are secured on the L298N. |
| **Car turns opposite to command** | Motor polarities swapped | Swap wire positions at L298N `OUT1/OUT2` or `OUT3/OUT4`. |
| **Wi-Fi AP doesn't appear** | ESP32 failed to boot or improper upload | Open Serial Monitor at `115200` baud to view debug messages. Press the `EN`/`RST` button. |
| **Web page fails to load at 192.168.4.1** | Device disconnected from AP or cellular data interference | Disable cellular mobile data on your phone while connected to the car's local Wi-Fi. |

---

## 📄 License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute for personal, educational, or commercial projects.
