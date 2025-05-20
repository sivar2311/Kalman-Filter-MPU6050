#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <KalmanFilter.h>
#include "WebPages.h"

// Replace with your network credentials
// Note: ESP32 will not connect with ssid such as "John's iPhone"; Change your ssid to be "Johns iPhone" for example (no apostrophe)
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Timer variables
unsigned long lastTime = 0;  
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;

// Create a sensor object
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float estimateGyroX = 0;
float estimateGyroY = 0;

float accX, accY, accZ;
float temperature;

KalmanFilter kalmanX(0.001, 0.003, 0.03);
KalmanFilter kalmanY(0.001, 0.003, 0.03);
KalmanFilter kalmanz(0.001, 0.003, 0.03);

float kalX = 0;
float kalY = 0;
float kalZ = 0;

// Initialize MPU6050
void initMPU(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
}

void initWebserver() {
  // Handle Web Server
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    gyroY=0;
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetX", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetY", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroY=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetZ", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/css", style_css); });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/javascript", script_js); });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/html", index_html); });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();
  Serial.printf("Open your browser and browse to http://%s/\r\n", WiFi.localIP().toString().c_str());
}

String getGyroReadings(){
  mpu.getEvent(&a, &g, &temp);
  
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;

  gyroX = g.gyro.x;
  gyroY = g.gyro.y;

  // Calculate rotational velocities for X and Y axis (rad/s)
  estimateGyroY = -atan2(accX, sqrt(accY*accY + accZ*accZ));
  estimateGyroX  = atan2(accY, accZ);

  // Kalman filter
  kalY = kalmanY.update(estimateGyroY, gyroY);
  kalX = kalmanX.update(estimateGyroX, gyroX);
  
  // No trigonometric equation exists to calculate rotational velocity in the z direction given gravitation accelerations
  // To simplify things, we check if the value is bigger than the rotation velocity when the accelerometer is static (not moving)
  // If that is true, then we consider that we have a valid reading, as in the accelerometer is non-static (is moving)
  float gyroZ_temp = g.gyro.z;
  if(abs(gyroZ_temp) > 0.03) {
    // current angle (rad) = last angle (rad) + angular velocity (rad/s) * time(s)
    gyroZ += gyroZ_temp * 0.01;
  }

  JsonDocument readings;

  readings["gyroX"] = String(kalX);
  readings["gyroY"] = String(kalY);
  readings["gyroZ"] = String(gyroZ);

  String jsonString;
  serializeJson(readings, jsonString);
  return jsonString;
}

String getAccReadings(){
  mpu.getEvent(&a, &g, &temp);
  // Get current acceleration values
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;

  JsonDocument readings;
  readings["accX"] = String(accX);
  readings["accY"] = String(accY);
  readings["accZ"] = String(accZ);
  String accString;
  serializeJson(readings, accString);
  return accString;
}

String getTemperature(){
  mpu.getEvent(&a, &g, &temp);
  temperature = temp.temperature;
  return String(temperature);
}

void handleGyro() {
  if ((millis() - lastTime) > gyroDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getGyroReadings().c_str(),"gyro_readings",millis());
    lastTime = millis();
  }
}

void handleAccelerometer() {
 if ((millis() - lastTimeAcc) > accelerometerDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getAccReadings().c_str(),"accelerometer_readings",millis());
    lastTimeAcc = millis();
  }
}

void handleTemperature() {
  if ((millis() - lastTimeTemperature) > temperatureDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getTemperature().c_str(),"temperature_reading",millis());
    lastTimeTemperature = millis();
  }
}

void handleMPU6050() {
  handleGyro();
  handleAccelerometer();
  handleTemperature();
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initMPU();
  initWebserver();
}

void loop() {
  handleMPU6050();
}