# ESP32 Weather Station

![Weather Station](https://img.icons8.com/color/96/000000/weather.png)

## Overview

This project is an ESP32-based weather station that measures temperature, humidity, pressure, and altitude using DHT11 and BMP180 sensors. The data is displayed on a web server hosted by the ESP32.

## Features

- **Temperature and Humidity**: Measured using the DHT11 sensor.
- **Pressure and Altitude**: Measured using the BMP180 sensor.
- **Wi-Fi Connectivity**: Connects to a Wi-Fi network to serve the data.
- **Web Server**: Displays the sensor data on a web page.

## Components

- **ESP32**: The main microcontroller.
- **DHT11**: Temperature and humidity sensor.
- **BMP180**: Pressure and altitude sensor.
- **Breadboard and Jumper Wires**: For connecting the components.

## Circuit Diagram

![Circuit Diagram](https://img.icons8.com/color/96/000000/electronics.png)

## Installation

1. **Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/ESP32-Weather.git
    cd ESP32-Weather
    ```

2. **Install PlatformIO**:
    Follow the instructions on the [PlatformIO website](https://platformio.org/install).

3. **Open the project**:
    Open the project folder in your preferred IDE (e.g., VSCode with the PlatformIO extension).

4. **Configure Wi-Fi credentials**:
    Update the `ssid` and `password` variables in `src/main.cpp` with your Wi-Fi credentials.

5. **Upload the code**:
    Connect your ESP32 to your computer and upload the code using PlatformIO.

## Usage

1. **Power the ESP32**:
    Connect the ESP32 to a power source.

2. **Access the web server**:
    Open a web browser and navigate to the IP address displayed in the serial monitor.

3. **View the data**:
    The web page will display the current temperature, humidity, pressure, and altitude.

## Screenshots

![Web Interface](https://img.icons8.com/color/96/000000/web.png)

## Libraries Used

- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [Sparkfun BMP180](https://github.com/sparkfun/SparkFun_BMP180_Breakout_Arduino_Library)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Icons by [Icons8](https://icons8.com)

![ESP32](https://img.icons8.com/color/96/000000/esp32.png)
