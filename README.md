# KalmanFilterMPU6050
This program uses an ESP32 to host a web server that displays readings from an MPU-6050 accelerometer and gyroscope sensor. The sensor orientation is also visualized using a rectangular prism on the web page. This program uses Visual Studio Code and the extension PlatformIO.

## Project Structure
This project is using PlatformIO with ESP IDF in Visual Studio Code. PlatformIO is just a extra set of tools on top of ESP IDF to make it easier to build a project.

```
ESP32-WiFi-Packets
  ├───.pio
  ├───.vscode
  ├───build
  ├───include
  │       lwip_hooks.h
  │       README
  │
  ├───lib
  ├───src
  │       CMakeLists.txt
  │       main.c
  │
  └───test
  │   CMakeLists.txt
  │   platformio.ini
  │   sdkconfig
  │   sdkconfig.esp32dev
```
