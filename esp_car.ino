/*
 * ============================================================================
 * ESP32 Wi-Fi Controlled RC Car
 * ============================================================================
 * Hardware Wiring:
 *   L298N Motor Driver | ESP32 Pin | Purpose
 *   -------------------|-----------|-----------------------------------------
 *   IN1                | GPIO 26   | Right Motor Forward
 *   IN2                | GPIO 27   | Right Motor Backward
 *   IN3                | GPIO 14   | Left Motor Forward
 *   IN4                | GPIO 12   | Left Motor Backward
 *   GND                | GND       | Common Ground (Tie ESP32 & L298N GND!)
 *
 * Web Controller Access:
 *   - Wi-Fi SSID: ESP32-RC-Car
 *   - Password  : password123
 *   - Web URL   : http://192.168.4.1
 * ============================================================================
 */

#include <WebServer.h>
#include <WiFi.h>

// Set your Wi-Fi credentials for the ESP32 Access Point
const char *ssid = "ESP32-RC-Car";
const char *password = "password123";

// Create a web server on port 80
WebServer server(80);

// Motor A (Right side)
const int IN1 = 26; // IN1 -> GPIO 26 (Right Motor Forward)
const int IN2 = 27; // IN2 -> GPIO 27 (Right Motor Backward)

// Motor B (Left side)
const int IN3 = 14; // IN3 -> GPIO 14 (Left Motor Forward)
const int IN4 = 12; // IN4 -> GPIO 12 (Left Motor Backward)

// HTML & CSS for the Web Controller (Mobile Touch Friendly)
const char *htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <title>ESP32 RC Car</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 40px; background-color: #121212; color: white; user-select: none; }
    .btn { background-color: #4CAF50; border: none; color: white; width: 80px; height: 80px; font-size: 30px; margin: 10px; border-radius: 15px; touch-action: manipulation; box-shadow: 0 4px #2e6c31; }
    .btn:active { background-color: #3e8e41; box-shadow: 0 1px #2e6c31; transform: translateY(3px); }
    .btn-stop { background-color: #f44336; box-shadow: 0 4px #982a22; }
    .btn-stop:active { background-color: #da190b; box-shadow: 0 1px #982a22; }
    .row { display: flex; justify-content: center; }
  </style>
</head>
<body>
  <h2>RC Controller</h2>
  <div class="row">
    <button class="btn" ontouchstart="sendCommand('F')" ontouchend="sendCommand('S')" onmousedown="sendCommand('F')" onmouseup="sendCommand('S')">▲</button>
  </div>
  <div class="row">
    <button class="btn" ontouchstart="sendCommand('L')" ontouchend="sendCommand('S')" onmousedown="sendCommand('L')" onmouseup="sendCommand('S')">◀</button>
    <button class="btn btn-stop" ontouchstart="sendCommand('S')" onmousedown="sendCommand('S')">■</button>
    <button class="btn" ontouchstart="sendCommand('R')" ontouchend="sendCommand('S')" onmousedown="sendCommand('R')" onmouseup="sendCommand('S')">▶</button>
  </div>
  <div class="row">
    <button class="btn" ontouchstart="sendCommand('B')" ontouchend="sendCommand('S')" onmousedown="sendCommand('B')" onmouseup="sendCommand('S')">▼</button>
  </div>
  
  <script>
    function sendCommand(cmd) {
      fetch('/' + cmd);
    }
  </script>
</body>
</html>
)rawliteral";

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  Serial.begin(115200);

  // Configure Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopMotors();

  // Start the Wi-Fi Access Point
  Serial.println("Starting Access Point...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Define Web Server Routes
  server.on("/", []() { server.send(200, "text/html", htmlPage); });

  server.on("/F", []() {
    Serial.println("Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    server.send(200);
  });

  server.on("/B", []() {
    Serial.println("Backward");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    server.send(200);
  });

  server.on("/L", []() {
    Serial.println("Left");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    server.send(200);
  });

  server.on("/R", []() {
    Serial.println("Right");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    server.send(200);
  });

  server.on("/S", []() {
    Serial.println("Stop");
    stopMotors();
    server.send(200);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() { server.handleClient(); }