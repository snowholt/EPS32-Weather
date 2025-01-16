#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <DHT.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#define DHTPIN 4     // Pin where the DHT11 is connected
#define DHTTYPE DHT11

const char* ssid = "Dark.Lumi";
const char* password = "C961E9C4DDCA";

DHT dht(DHTPIN, DHTTYPE);
SFE_BMP180 bmp180;
AsyncWebServer server(80);

float temperatureReadings[60] = {0};
float humidityReadings[60] = {0};
float pressureReadings[60] = {0};
float altitudeReadings[60] = {0};
int readingIndex = 0;

void setup() {
  Serial.begin(9600); // Change the baud rate to 9600
  dht.begin();
  
  Wire.begin(); // Initialize I2C communication (SDA to SDA, SCL to SCL)

  if (bmp180.begin()) {
    Serial.println("BMP180 init success");
  } else {
    Serial.println("BMP180 init fail");
    while (1); // Pause forever.
  }

  // Demonstrate power management
  bmp180.powerDown();
  delay(1000);
  bmp180.powerUp();

  // Demonstrate calibration
  bmp180.calibrateSensor();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Get sensor information and print to serial
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  char status;
  double T = 0, P = 0, A = 0;

  // Non-blocking temperature measurement
  status = bmp180.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp180.getTemperature(T);
    if (status != 0) {
      // Non-blocking pressure measurement
      status = bmp180.startPressure(3);
      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);
        if (status != 0) {
          A = bmp180.altitude(P, 1013.25);
        }
      }
    }
  }

  Serial.print("DHT11 Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("DHT11 Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("BMP180 Temperature: ");
  Serial.print(T);
  Serial.println(" °C");
  Serial.print("BMP180 Pressure: ");
  Serial.print(P / 100.0); // Convert to hPa
  Serial.println(" hPa");
  Serial.print("BMP180 Altitude: ");
  Serial.print(A);
  Serial.println(" m");

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", R"rawliteral(
      <!DOCTYPE HTML><html>
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>ESP32 Weather Station</title>
        <style>
          body {
            font-family: Arial;
            text-align: center;
            background-color: #f0f0f0;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
          }
          h1 {
            margin-bottom: 20px;
          }
          .data-container {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 50px; /* Add a 50px gap between the boxes */
            width: 80%;
            max-width: 600px;
          }
          .data-box {
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            padding: 20px;
            text-align: center;
            border: 2px solid #007BFF; /* Add a blue frame around the box */
            width: 350px; /* Ensure all boxes have the same width */
            height: 130px; /* Ensure all boxes have the same height */
          }
          .data-box .icon {
            font-size: 2em;
            margin-bottom: 10px;
          }
          .data-box .data {
            font-size: 1.2em;
          }
          .temperature {
            color: #FF5733; /* Set color for temperature */
          }
          .humidity {
            color: #008B8B; /* Set color for humidity */
          }
          .pressure {
            color:rgba(126, 41, 129, 0.69); /* Set color for pressure */
          }
          .altitude {
            color: #2ECC40; /* Set color for altitude */
          }
        </style>
        <script>
          function updateData() {
            fetch("/data")
              .then((response) => response.json())
              .then((data) => {
                document.getElementById("temperature").innerText = data.temperature;
                document.getElementById("bmpTemperature").innerText = data.bmpTemperature;
                document.getElementById("humidity").innerText = data.humidity;
                document.getElementById("pressure").innerText = data.pressure;
                document.getElementById("altitude").innerText = data.altitude;
              })
              .catch((error) => console.error("Error fetching data:", error));
          }

          setInterval(updateData, 1500);
          updateData(); // Initial call to populate data
        </script>
      </head>
      <body>
        <h1>ESP32 Weather Station</h1>
        <div class="data-container">
          <div class="data-box">
            <div class="icon"><img src="https://img.icons8.com/color/48/000000/thermometer.png" alt="Temperature Icon"></div>
            <div class="data temperature">Temperature (DHT11): <span id="temperature">--</span> &deg;C</div>
          </div>
          <div class="data-box">
            <div class="icon"><img src="https://img.icons8.com/color/48/000000/thermometer.png" alt="Temperature Icon"></div>
            <div class="data temperature">Temperature (BMP180): <span id="bmpTemperature">--</span> &deg;C</div>
          </div>
          <div class="data-box">
            <div class="icon"><img src="https://img.icons8.com/color/48/000000/humidity.png" alt="Humidity Icon"></div>
            <div class="data humidity">Humidity: <span id="humidity">--</span> %</div>
          </div>
          <div class="data-box">
            <div class="icon"><img src="https://img.icons8.com/color/48/000000/barometer.png" alt="Pressure Icon"></div>
            <div class="data pressure">Pressure: <span id="pressure">--</span> hPa</div>
          </div>
          <div class="data-box">
            <div class="icon"><img src="https://img.icons8.com/color/48/000000/mountain.png" alt="Altitude Icon"></div>
            <div class="data altitude">Altitude: <span id="altitude">--</span> m</div>
          </div>
        </div>
      </body>
      </html>
    )rawliteral");
  });

  // Serve the sensor data
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    char status;
    double T = 0, P = 0, A = 0;

    // Non-blocking temperature measurement
    status = bmp180.startTemperature();
    if (status != 0) {
      delay(status);
      status = bmp180.getTemperature(T);
      if (status != 0) {
        // Non-blocking pressure measurement
        status = bmp180.startPressure(3);
        if (status != 0) {
          delay(status);
          status = bmp180.getPressure(P, T);
          if (status != 0) {
            A = bmp180.altitude(P, 1013.25);
          }
        }
      }
    }

    temperatureReadings[readingIndex] = temperature;
    humidityReadings[readingIndex] = humidity;
    pressureReadings[readingIndex] = P / 100.0; // Convert to hPa
    altitudeReadings[readingIndex] = A;
    readingIndex = (readingIndex + 1) % 60;

    String json = "{\"temperature\":" + String(temperature) + ",\"bmpTemperature\":" + String(T) + ",\"humidity\":" + String(humidity) + ",\"pressure\":" + String(P / 100.0) + ",\"altitude\":" + String(A) + "}";

    request->send(200, "application/json", json);
  });

  server.begin();
  Serial.println("HTTP server started"); // Confirmation message
}

void loop() {
  // No need for code in the loop when using AsyncWebServer
}
