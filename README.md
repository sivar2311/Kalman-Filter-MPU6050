# KalmanFilterMPU6050
This program uses an ESP32 to host a web server that displays readings from an MPU-6050 accelerometer and gyroscope sensor. The sensor orientation is also visualized using a rectangular prism on the web page. The gyroscope sensor readings (pitch and roll at least) are filtered using a Kalman filter. This program uses Visual Studio Code and the extension PlatformIO.

 <img src="https://s3.gifyu.com/images/bS5m7.gif
"/>

## Project Structure
This project is using PlatformIO with the Arduino framework in Visual Studio Code. PlatformIO is just a extra set of tools to make it easier to build a project.

```
Kalman-Filter-MPU6050
  ├───.pio
  ├───.vscode
  ├───data
  │        index.html
  │        script.js
  │        style.css
  ├───include
  ├───lib
  │    └───KalmanFilter
  │            KalmanFilter.cpp
  │            KalmanFilter.h
  ├───src
  │        ESP32_MPU_6050_Web_Server.cpp
  │
  └───test
  │   platformio.ini
```

<p align="center">
  <img src="https://github.com/user-attachments/assets/b5f7d076-f044-4ed2-8564-c85cd385ab7b"><br>
  Folder structure in VS Code after building.
</p>
