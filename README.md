# ESP32 Soil Moisture Telemetry Monitor 🌱

A low-power, cloud-connected soil moisture monitoring system using an **ESP32**, a **2-prong soil sensor**, and **Telemetry Harbor**. The project wakes at set intervals, measures soil moisture, sends the data to the cloud, and then returns to deep sleep to conserve battery.



## Overview

This project wakes up periodically, reads soil moisture percentage from the 2-prong sensor, sends it to Telemetry Harbor, and returns to deep sleep for ultra-low-power operation. Ideal for **gardens, potted plants, and soil monitoring projects**.

### Features

* **Ultra-Low Power**: Uses **ESP32 Deep Sleep mode** to extend battery life.
* **2-Prong Soil Sensor Support**: Accurate analog readings from standard 2-prong sensors.
* **Cloud Connectivity**: Sends readings to Telemetry Harbor using the official SDK.
* **Timestamped Data**: Each reading is recorded with an ISO8601 UTC timestamp.
* **Grafana Dashboards**: Ready for visualization with Telemetry Harbor.



## Hardware Requirements

| Component           | Description                                    | Approximate Cost                       |
| ------------------- | ---------------------------------------------- | -------------------------------------- |
| ESP32               | Development board (NodeMCU ESP32, WROOM, etc.) | $3.82 (AliExpress) / $9.90 (Amazon US) |
| 2-Prong Soil Sensor | Capacitive or resistive soil moisture probe    | $1.50–$3.00                            |
| Jumper wires        | For connecting sensor                          | $0.42 (AliExpress) / $5.49 (Amazon US) |
| Micro USB cable     | For power & programming                        | $3.00 (AliExpress) / $5.00 (Amazon US) |
| Battery (optional)  | For standalone, low-power operation            | Cost varies                            |



## Software Requirements

* Arduino IDE (1.8.13 or newer) or PlatformIO
* ESP32 board support package
* Required libraries:

  * **Telemetry Harbor SDK**
  * WiFi Library (built into ESP32 core)
* [Telemetry Harbor](https://telemetryharbor.com) account



## Wiring Diagram

Connect the 2-prong soil sensor to the ESP32:

| Soil Sensor Pin | ESP32 Pin | Function     |
| --------------- | --------- | ------------ |
| VCC             | 3.3V      | Power        |
| GND             | GND       | Ground       |
| Signal / Output | GPIO 34   | Analog input |

> Notes:
>
> * Ensure both sensor prongs share a **common ground with ESP32**.
> * Capacitive sensors are recommended for long-term use; resistive sensors may corrode over time.
> * Analog values can be noisy; consider averaging multiple readings.



## Setup Instructions

### 1. Hardware Assembly

1. Connect the 2-prong soil sensor to the ESP32 according to the wiring diagram.
2. Connect the ESP32 to your computer via USB.

### 2. Software Setup

1. Install Arduino IDE.
2. Add ESP32 board support:

   * Preferences → Additional Boards Manager URLs:

     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
3. Install required libraries via Library Manager:

   * Telemetry Harbor SDK
   * WiFi Library (built-in)

### 3. Telemetry Harbor Configuration

1. Create a [Telemetry Harbor](https://telemetryharbor.com) account.
2. Create a new Harbor called `"ESP32_Soil_Monitor"` (or your preferred name).
3. Copy the **API ENDPOINT** and **API Key** from the Harbor details page.

### 4. Configure and Upload the Code

1. Open your soil sensor `.ino` file in Arduino IDE.
2. Update the following variables at the top of the file:

   * WiFi SSID and password
   * Telemetry Harbor API Endpoint URL and API Key
   * `shipId` (e.g., `"Garden 1"`)
3. Select your ESP32 board and COM port.
4. Upload the code.

### 5. Visualize Your Data

1. Log in to your Telemetry Harbor account.
2. Navigate to the Harbor details page and Grafana dashboard.
3. Monitor soil moisture in real time and over time.



## Customization

### Sleep Interval

Change how often the ESP32 wakes up:

```cpp
const int SLEEP_INTERVAL_SEC = 300; // 5 minutes
```

### Sensor Calibration

Adjust offset for more accurate readings:

```cpp
const float SOIL_OFFSET = 0.0; // +/- percentage for calibration
```

### Debug Mode

Add Serial prints for testing:

```cpp
Serial.printf("Raw ADC: %d, Moisture: %.2f%%\n", raw, moisture);
```

> Disable deep sleep during debugging to view Serial output.



## Troubleshooting

* **Sensor gives constant values**: Verify wiring, common ground, and analog output.
* **WiFi connection fails**: Check credentials; device retries on next wake cycle.
* **Data not appearing in Telemetry Harbor**: Verify API Key and Endpoint URL.



## Contributing

Contributions are welcome! Submit Pull Requests to improve features or optimize power usage.



## License

MIT License – see LICENSE file for details.



## Acknowledgments

* [Espressif](https://www.espressif.com) for ESP32 platform
* [Telemetry Harbor](https://telemetryharbor.com) for IoT SDK and dashboard

