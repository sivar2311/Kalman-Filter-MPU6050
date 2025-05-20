# KalmanFilterMPU6050
This program uses an ESP32 to host a web server that displays readings from an MPU-6050 accelerometer and gyroscope sensor. The sensor orientation is also visualized using a rectangular prism on the web page. The gyroscope sensor readings (pitch and roll at least) are filtered using a Kalman filter. This program uses Visual Studio Code and the extension PlatformIO.

<p align="center">
  <img src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExZTF2endpYW4xaG40anFkdDg2djZ5ZWxrczZsOTNtdzFiNTZvZjJueCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/jeLeWuEFeydKm17wbs/giphy.gif"><br>
  Physical Demonstration<br>
  <a href="https://www.youtube.com/watch?v=UjhBxuVpexo">Youtube Version</a>
</p>

## Project Structure
This project is using PlatformIO with the Arduino framework in Visual Studio Code. PlatformIO is just a extra set of tools to make it easier to build a project.

```
Kalman-Filter-MPU6050
.
├── lib
│   └── KalmanFilter
│       ├── KalmanFilter.cpp
│       └── KalmanFilter.h
├── src
│   ├── ESP32_MPU_6050_Web_Server.cpp
│   ├── WebPages.cpp
│   └── WebPages.h
├── .gitignore
├── platformio.ini
└── README.md
```

# Modifications
This repository is a fork of https://github.com/Mohamed1628/Kalman-Filter-MPU6050 with the following modifications and updates
- Updated libraries are used (`esp32async/ESPAsyncWebServer`)
- Moved from `arduino-libraries/Arduino_JSON` to `bblanchon/ArduinoJson`
- Removed the datafiles "index.html", "script.js" and "style.css" from local filesystem directly into the code - See [`src/WebPages.h`](./src/WebPages.h) and [`src/WebPages.cpp`](./src/WebPages.cpp)
- Code restructuring